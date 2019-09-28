HERE = File.expand_path(File.dirname(__FILE__)) + '/'

require 'rake'
require 'rake/clean'
require 'rake/testtask'
require './rakefile_helper'

include RakefileHelpers

# Load default configuration, for now
DEFAULT_CONFIG_FILE = 'gcc.yml'
configure_toolchain(DEFAULT_CONFIG_FILE)

REQUIRED_DIRS = [ './ut_build', './ut_build/mocks' ]
REQUIRED_DIRS.each do |v|
  directory v
end

task :unit do
  # Clean first before we begin; this will also delete old code coverage files
  Rake::Task["clean"].execute

  # User can specify a single test "rake unit single_file=test_code/test_sample.c"
  single = ENV['single_file']

  # If the single file was not specified
  if single.nil?
    report "Running Unit-Tests"
    run_tests(get_unit_test_files)
  # If file was found, run this single test
  else
    report "Running single unit-test: #{single}"
    run_tests(FileList.new(single))
  end
end

desc "Generate test summary"
task :summary do
  report_summary
end

desc "Build and test Unity"
task :all => [:clean, :unit, :summary]
task :default => REQUIRED_DIRS + [:clobber, :all]
task :ci => [:default]
task :cruise => [:default]

desc "Load configuration"
task :config, :config_file do |t, args|
  configure_toolchain(args[:config_file])
end

desc "Return error on Failures"
task :strict do
  $return_error_on_failures = true
end
