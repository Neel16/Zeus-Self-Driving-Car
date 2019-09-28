require 'yaml'
require 'fileutils'
require './install_unity_cmock/vendor/unity/auto/unity_test_summary'
require './install_unity_cmock/vendor/unity/auto/generate_test_runner'
require './install_unity_cmock/vendor/unity/auto/colour_reporter'

module RakefileHelpers

  $return_error_on_failures = false
  C_EXTENSION = '.c'

  def line()
      report "------------------------------------------------------------------------------------------------"
  end

  def load_configuration(config_file)
    $cnfg_file = config_file
    $yml_cfg = YAML.load(File.read($cnfg_file))
    $colour_output = false unless $yml_cfg['colour']
  end

  def configure_clean
    CLEAN.include($yml_cfg['compiler']['build_path'] + '*.*') unless $yml_cfg['compiler']['build_path'].nil?
    CLEAN.include($yml_cfg['compiler']['build_path'] + 'mocks/*.*') unless $yml_cfg['compiler']['build_path'].nil?
  end

  # Used by rakefile.rb to load configuration
  def configure_toolchain(config_file=DEFAULT_CONFIG_FILE)
    config_file += '.yml' unless config_file =~ /\.yml$/
    load_configuration(config_file)
    configure_clean
  end

  # Used by rakefile.rb to get a list of all files in the UT directory
  def get_unit_test_files
    path = $yml_cfg['compiler']['unit_tests_path'] + 'test_*' + C_EXTENSION
    path.gsub!(/\\/, '/')
    FileList.new(path)
  end

  # Get a list of all includes from the gcc.yml
  def get_local_include_dirs
    include_dirs = $yml_cfg['compiler']['includes']['items'].dup
    include_dirs.delete_if {|dir| dir.is_a?(Array)}
    return include_dirs
  end

  # This extracts #includes from a header file and returns the list
  def extract_headers(filename)
    includes = []
    lines = File.readlines(filename)
    lines.each do |line|
      m = line.match(/^\s*#include\s+\"\s*(.+\.[hH])\s*\"/)
      if not m.nil?
        includes << m[1]
      end
    end
    return includes
  end

  # This finds the source file of a header
  def find_source_file(header, paths)
    paths.each do |dir|
      src_file = dir + header.ext(C_EXTENSION)
      if (File.exists?(src_file))
        return src_file
      end
    end
    return nil
  end

  # This locates a header file from the include paths
  def find_header_file(header, paths)
    paths.each do |dir|
      src_file = dir + header.ext('.h')
      if (File.exists?(src_file))
        return src_file
      end
    end
    return nil
  end

  # Not sure what this does :(
  def tackit(strings)
    case(strings)
      when Array
        "\"#{strings.join}\""
      when /^-/
        strings
      when /\s/
        "\"#{strings}\""
      else
        strings
    end
  end

  def squash(prefix, items)
    result = ''
    items.each { |item| result += " #{prefix}#{tackit(item)}" }
    return result
  end

  # This uses the parsed yml file to find compiler fields
  def build_compiler_fields
    command  = tackit($yml_cfg['compiler']['path'])
    if $yml_cfg['compiler']['defines']['items'].nil?
      defines  = ''
    else
      defines  = squash($yml_cfg['compiler']['defines']['prefix'], $yml_cfg['compiler']['defines']['items'])
    end
    options  = squash('', $yml_cfg['compiler']['options'])
    includes = squash($yml_cfg['compiler']['includes']['prefix'], $yml_cfg['compiler']['includes']['items'])
    includes = includes.gsub(/\\ /, ' ').gsub(/\\\"/, '"').gsub(/\\$/, '') # Remove trailing slashes (for IAR)
    return {:command => command, :defines => defines, :options => options, :includes => includes}
  end

  # This uses the yml file to compile a file
  def compile(file, defines=[])
    compiler = build_compiler_fields
    cmd_str  = "#{compiler[:command]}#{compiler[:defines]}#{compiler[:options]}#{compiler[:includes]} #{file} " +
               "#{$yml_cfg['compiler']['object_files']['prefix']}#{$yml_cfg['compiler']['object_files']['destination']}"
    obj_file = "#{File.basename(file, C_EXTENSION)}#{$yml_cfg['compiler']['object_files']['extension']}"
    execute(cmd_str + obj_file, false, false)
    return obj_file
  end

  # Helper function of link_it()
  def build_linker_fields
    command  = tackit($yml_cfg['linker']['path'])
    if $yml_cfg['linker']['options'].nil?
      options  = ''
    else
      options  = squash('', $yml_cfg['linker']['options'])
    end

    if ($yml_cfg['linker']['includes'].nil? || $yml_cfg['linker']['includes']['items'].nil?)
      includes = ''
    else
      includes = squash($yml_cfg['linker']['includes']['prefix'], $yml_cfg['linker']['includes']['items'])
    end

    includes = includes.gsub(/\\ /, ' ').gsub(/\\\"/, '"').gsub(/\\$/, '') # Remove trailing slashes (for IAR)
    return {:command => command, :options => options, :includes => includes}
  end

  # This invokes the linker to compile the executable
  def link_it(exe_name, obj_list)
    linker = build_linker_fields
    cmd_str = "#{linker[:command]}#{linker[:includes]} " +
      (obj_list.map{|obj|"#{$yml_cfg['linker']['object_files']['path']}#{obj} "}).join +
      $yml_cfg['linker']['bin_files']['prefix'] + ' ' +
      $yml_cfg['linker']['bin_files']['destination'] +
      exe_name + $yml_cfg['linker']['bin_files']['extension'] + " #{linker[:options]}"
    execute(cmd_str, false, false)
  end

  # Execute a system command on the OS
  def execute(command_string, print_cmd, verbose=true, ok_to_fail=false)
    if print_cmd
      report command_string
    end

    output =`#{command_string}`.chomp
    report(output) if (verbose && !output.nil? && (output.length > 0))
    unless $?.exitstatus.zero? || ok_to_fail
      raise "Command failed. (Returned #{$?.exitstatus})"
    end
    return output
  end

  def report_summary
    summary = UnityTestSummary.new
    summary.root = HERE
    results_glob = "#{$yml_cfg['compiler']['build_path']}*.test*"
    results_glob.gsub!(/\\/, '/')
    results = Dir[results_glob]
    summary.targets = results
    report summary.run
    raise "There were failures" if (summary.failures > 0) && $return_error_on_failures
  end

  # This locates all *.gcda files to the corresponding files at the "code"
  # directory and prints the coverage for those
  def output_coverage
    line()
    report "Brief code coverage report; use gen_coverage.sh for more details"
    line()

    # Form the path of the source code under test
    src_path = $yml_cfg['compiler']['source_path']
    src_path = File.join(src_path, "**")
    src_path = File.join(src_path, "*.c")

    full_report = ""
    ut_output_dir = $yml_cfg['compiler']['build_path']

    # For each file in the source code directory
    Dir[src_path].each do |fname|
      fname_only = File.basename(fname)

      # Remove the *.c and add .gcda extension
      gcda = File.basename(fname_only, ".c") + ".gcda"

      # Attempt to find the code_under_test.gcda at the ut build directory
      gcda_path = File.join(ut_output_dir, gcda)
      if (File.exists?(gcda_path))
        # Get coverage, but do not output a file, and filter the "Lines executed:n%" msg
        cmd = "gcov --no-output " + gcda_path # + " | grep '%'"
        cmd_output = execute(cmd, false, false)
        
        full_report += cmd_output + "\n"
        '''
        # Print back to the user
        msg = "%48s" % [fname]
        cov_pct = cmd_output.scan(/\d+/).first.to_i
        if cov_pct < 90
          full_report += "#{msg} coverage: \033[31m#{cmd_output}\033[0m\n"
        else
          full_report += "#{msg} coverage: \033[32m#{cmd_output}\033[0m\n"
        end
        '''
      end
    end

    if full_report == ""
      report "Error generating code coverage report"
    else
      report full_report
    end 
    line()
  end

  def run_tests(test_files)
    # Tack on TEST define for compiling unit tests
    load_configuration($cnfg_file)
    test_defines = ['TEST']
    $yml_cfg['compiler']['defines']['items'] = [] if $yml_cfg['compiler']['defines']['items'].nil?
    $yml_cfg['compiler']['defines']['items'] << 'TEST'

    include_dirs = get_local_include_dirs

    # Build and execute each unit test
    test_files.each do |test|
      line()
      report "Compile and run: #{test}"
      obj_list = []

      # Detect dependencies and build required required modules
      header_list = extract_headers(test) + ['cmock.h']
      header_list.each do |header|

        #create mocks if needed
        if (header =~ /Mock/)
          require "./install_unity_cmock/lib/cmock.rb"

          @cmock ||= CMock.new($cnfg_file)

          mock_file = header.gsub('Mock','')
          src_file = find_header_file(mock_file, include_dirs)

          @cmock.setup_mocks(src_file)
        end
      end # header_list.each do |header|

      #compile all mocks
      header_list.each do |header|
        #compile source file header if it exists
        src_file = find_source_file(header, include_dirs)
        if !src_file.nil?
          obj_list << compile(src_file, test_defines)
        end
      end

      # Build the test runner (generate if configured to do so)
      test_base = File.basename(test, C_EXTENSION)
      runner_name = test_base + '_Runner.c'
      if $yml_cfg['compiler']['runner_path'].nil?
        runner_path = $yml_cfg['compiler']['build_path'] + runner_name
        test_gen = UnityTestRunnerGenerator.new($cnfg_file)
        test_gen.run(test, runner_path)
      else
        runner_path = $yml_cfg['compiler']['runner_path'] + runner_name
      end

      obj_list << compile(runner_path, test_defines)
      obj_list << compile(test, test_defines)
      link_it(test_base, obj_list)

      # Execute unit test and generate results file
      executable = $yml_cfg['linker']['bin_files']['destination'] + test_base + $yml_cfg['linker']['bin_files']['extension']
      
      output = execute(executable, true, true, true)
      test_results = $yml_cfg['compiler']['build_path'] + test_base
      if output.match(/OK$/m).nil?
        test_results += '.testfail'
      else
        test_results += '.testpass'
      end
      File.open(test_results, 'w') { |f| f.print output }
    end # test_files.each do |test|

    # After all tests run, output coverage for each source file's *.gcno we can find
    output_coverage()
  end # run_tests()

end # RakefileHelpers
