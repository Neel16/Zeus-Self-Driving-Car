package com.example.mapdemo;

import android.Manifest;
import android.app.Dialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.location.Location;
import android.os.Bundle;
import android.os.Looper;
import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;
import android.text.TextUtils;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;
import android.app.Activity;

import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.GooglePlayServicesUtil;
import com.google.android.gms.location.FusedLocationProviderClient;
import com.google.android.gms.location.LocationCallback;
import com.google.android.gms.location.LocationRequest;
import com.google.android.gms.location.LocationResult;
import com.google.android.gms.location.LocationServices;
import com.google.android.gms.location.LocationSettingsRequest;
import com.google.android.gms.location.SettingsClient;
import com.google.android.gms.maps.CameraUpdate;
import com.google.android.gms.maps.CameraUpdateFactory;
import com.google.android.gms.maps.GoogleMap;
import com.google.android.gms.maps.OnMapReadyCallback;
import com.google.android.gms.maps.SupportMapFragment;
import com.google.android.gms.maps.model.BitmapDescriptor;
import com.google.android.gms.maps.model.BitmapDescriptorFactory;
import com.google.android.gms.maps.model.LatLng;
import com.google.android.gms.maps.model.MarkerOptions;
import com.google.android.gms.tasks.OnFailureListener;
import com.google.android.gms.tasks.OnSuccessListener;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Set;
import java.util.UUID;

import permissions.dispatcher.NeedsPermission;
import permissions.dispatcher.RuntimePermissions;

import static com.google.android.gms.location.LocationServices.getFusedLocationProviderClient;

@RuntimePermissions
public class MapDemoActivity extends AppCompatActivity {

    private Button button;
    private Button destination;
    private SupportMapFragment mapFragment;
    //BT
    private static final String TAG ="StartScreen";
    String device_name = "HC-05";
    boolean status;
    private Button mConnect;
    private Button mStart;
    private Button mStop;
    InputStream tINPUT;
    OutputStream tOUTPUT;
    BluetoothDevice pairedDevice = null;
    BluetoothSocket mBTsocket = null;
    BluetoothDevice BoardSJONE = null;

    BluetoothAdapter ZeusAdapter;   //adapter for bluetooth
    //BT DECLARATION END

    MarkerOptions marker = new MarkerOptions();
    LatLng Dest_point;
    private GoogleMap map;
    private LocationRequest mLocationRequest;
    Location mCurrentLocation;
    private long UPDATE_INTERVAL = 60000;  /* 60 secs */
    private long FASTEST_INTERVAL = 5000; /* 5 secs */

    private final static String KEY_LOCATION = "location";

    /*
     * Define a request code to send to Google Play services This code is
     * returned in Activity.onActivityResult
     */
    private final static int CONNECTION_FAILURE_RESOLUTION_REQUEST = 9000;


    private final BroadcastReceiver ZeusReciever = new BroadcastReceiver() { //Broadcast reciever function to log states
        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();

            if(action.equals(ZeusAdapter.ACTION_STATE_CHANGED)){
                final int state = intent.getIntExtra(BluetoothAdapter.EXTRA_STATE, ZeusAdapter.ERROR);

                switch(state){
                    case BluetoothAdapter.STATE_OFF:
                        Log.d(TAG,"onReceive: STATE OFF");
                        break;
                    case BluetoothAdapter.STATE_TURNING_OFF:
                        Log.d(TAG, "ZeusReciever: STATE TURNING OFF");
                        break;
                    case BluetoothAdapter.STATE_ON:
                        Log.d(TAG,"ZeusReciever: STATE ON");
                        break;
                    case BluetoothAdapter.STATE_TURNING_ON:
                        Log.d(TAG,"ZeusReciever: STATE TURNING ON");

                }
            }
        }
    };

    @Override
    protected void onDestroy(){
        super.onDestroy();
        unregisterReceiver(ZeusReciever);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.map_demo_activity);

        if (TextUtils.isEmpty(getResources().getString(R.string.google_maps_api_key))) {
            throw new IllegalStateException("You forgot to supply a Google Maps API key");
        }

        if (savedInstanceState != null && savedInstanceState.keySet().contains(KEY_LOCATION)) {
            // Since KEY_LOCATION was found in the Bundle, we can be sure that mCurrentLocation
            // is not null.
            mCurrentLocation = savedInstanceState.getParcelable(KEY_LOCATION);
        }

        mapFragment = ((SupportMapFragment) getSupportFragmentManager().findFragmentById(R.id.map));
        if (mapFragment != null) {
            mapFragment.getMapAsync(new OnMapReadyCallback() {
                @Override
                public void onMapReady(GoogleMap map) {
                    loadMap(map);
                }
            });
        } else {
            Toast.makeText(this, "Error - Map Fragment was null!!", Toast.LENGTH_SHORT).show();
        }


        //BT
        mConnect = (Button) findViewById(R.id.Connect);
        mStart = (Button) findViewById(R.id.Start);
        mStop = (Button) findViewById(R.id.Stop);

        mConnect.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Toast.makeText(getApplicationContext(),"CONNECTING",Toast.LENGTH_SHORT).show();
                connect_to_board();
            }

        });

        mStart.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                send_to_BT(1);
            }
        });
        mStop.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                send_to_BT(2);
            }
        });


        destination= (Button)findViewById(R.id.Destination);
        destination.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                String msg = Double.toString(Dest_point.latitude) + "," + Double.toString(Dest_point.longitude) + "@";
                send_coordinates(msg);
               Toast.makeText(getApplicationContext(),msg,Toast.LENGTH_SHORT).show();
            }
        });

        //

    }

    @Override
    protected void onStart(){
        super.onStart();
        //client.connect();
        ZeusAdapter = BluetoothAdapter.getDefaultAdapter();
        status = ZeusAdapter.isEnabled();
        ZeusAdapter.startDiscovery();
        if(status)
        {
            Toast.makeText(getApplicationContext(),"ENABLED",Toast.LENGTH_SHORT).show();
            registerReceiver(ZeusReciever, new IntentFilter(BluetoothDevice.ACTION_FOUND));
        }
        else
        {
            Toast.makeText(getApplicationContext(),"NOT READY",Toast.LENGTH_SHORT).show();

        }
    }

    public void connect_to_board()
    {
        UUID BTMODULEUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB"); // "random" unique identifier
        Set<BluetoothDevice> devices = ZeusAdapter.getBondedDevices();
        for(BluetoothDevice device : devices){
            if(device_name.equals(device.getName()))
            {
                Toast.makeText(getApplicationContext(),"ZEUS AVAILABLE",Toast.LENGTH_LONG).show();
                BoardSJONE=device;
            }
            else{
                Toast.makeText(getApplicationContext(),"NotPaired yet",Toast.LENGTH_SHORT).show();

            }
        }
        try{
            mBTsocket = BoardSJONE.createInsecureRfcommSocketToServiceRecord(BTMODULEUUID);
            mBTsocket.connect();
            pairedDevice = BoardSJONE;
            Toast.makeText(getApplicationContext(),"Zeus Connected",Toast.LENGTH_LONG).show();


        }catch(IOException ioe)
        {
            Toast.makeText(getApplicationContext(),"COULDNT CONNECT",Toast.LENGTH_SHORT).show();
            pairedDevice = null;
        }


    }

    public void send_to_BT(int state)
    {
        if(pairedDevice != null && mBTsocket != null)
        {
            try {
                tOUTPUT = mBTsocket.getOutputStream();
                tOUTPUT.write(state);
                tOUTPUT.flush();

                Toast.makeText(getApplicationContext(), "SENT"+state, Toast.LENGTH_LONG).show();
            }catch(IOException ioe)
            {
                Toast.makeText(getApplicationContext(),"NOT SENT",Toast.LENGTH_LONG).show();

            }

        }
    }
   public void send_coordinates(String string) {
       if (pairedDevice != null && mBTsocket != null) {
           try {
               tOUTPUT = mBTsocket.getOutputStream();
               tOUTPUT.write(string.getBytes());
               tOUTPUT.flush();
               Toast.makeText(getApplicationContext(), "Destination:" + string, Toast.LENGTH_SHORT).show();
           } catch (IOException ioe) {
               Toast.makeText(getApplicationContext(), "NOT SENT", Toast.LENGTH_LONG).show();

           }
       }
   }
//    public void  send_destination(){
//        StartScreen Dest = new StartScreen(StartScreen);
//        Dest.send_to_BT(3);
//    }

    protected void loadMap(final GoogleMap googleMap) {
        map = googleMap;
        if (map != null) {
            // Map is ready
            Toast.makeText(this, "Map Fragment was loaded properly!", Toast.LENGTH_SHORT).show();
            MapDemoActivityPermissionsDispatcher.getMyLocationWithPermissionCheck(this);
            MapDemoActivityPermissionsDispatcher.startLocationUpdatesWithPermissionCheck(this);
        } else {
            Toast.makeText(this, "Error - Map was null!!", Toast.LENGTH_SHORT).show();
        }
        map.setOnMapClickListener(new GoogleMap.OnMapClickListener() { //marker added :neel
            @Override
            public void onMapClick(LatLng point) {


                Toast.makeText(getApplicationContext(),"DESTINATION MARKED",Toast.LENGTH_LONG).show();

                marker.position(point);
                marker.title(point.latitude + "," + point.longitude);  // point is an object of LatLng class , LAT and LON both are Doubles.
                //marker.title("Destination");
                marker.draggable(true);

                marker.icon(BitmapDescriptorFactory.fromResource(R.drawable.flash));
                //Destination_location.setLatitude(point.latitude);
                //Destination_location.setLongitude(point.longitude);
                Dest_point=point;

                map.clear();
                map.addMarker(marker);

            }
        });
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        MapDemoActivityPermissionsDispatcher.onRequestPermissionsResult(this, requestCode, grantResults);
    }

    @SuppressWarnings({"MissingPermission"})
    @NeedsPermission({Manifest.permission.ACCESS_FINE_LOCATION, Manifest.permission.ACCESS_COARSE_LOCATION})
    void getMyLocation() {
        map.setMyLocationEnabled(true);

        FusedLocationProviderClient locationClient = getFusedLocationProviderClient(this);
        locationClient.getLastLocation()
                .addOnSuccessListener(new OnSuccessListener<Location>() {
                    @Override
                    public void onSuccess(Location location) {
                        if (location != null) {
                            onLocationChanged(location);
                        }
                    }
                })
                .addOnFailureListener(new OnFailureListener() {
                    @Override
                    public void onFailure(@NonNull Exception e) {
                        Log.d("MapDemoActivity", "Error trying to get last GPS location");
                        e.printStackTrace();
                    }
                });
    }

    /*
     * Called when the Activity becomes visible.
    */
//    @Override
//    protected void onStart() {
//        super.onStart();
//    }

    /*
     * Called when the Activity is no longer visible.
	 */
    @Override
    protected void onStop() {
        super.onStop();
    }

    private boolean isGooglePlayServicesAvailable() {
        // Check that Google Play services is available
        int resultCode = GooglePlayServicesUtil.isGooglePlayServicesAvailable(this);
        // If Google Play services is available
        if (ConnectionResult.SUCCESS == resultCode) {
            // In debug mode, log the status
            Log.d("Location Updates", "Google Play services is available.");
            return true;
        } else {
            // Get the error dialog from Google Play services
            Dialog errorDialog = GooglePlayServicesUtil.getErrorDialog(resultCode, this,
                    CONNECTION_FAILURE_RESOLUTION_REQUEST);

            // If Google Play services can provide an error dialog
            if (errorDialog != null) {
                // Create a new DialogFragment for the error dialog
                ErrorDialogFragment errorFragment = new ErrorDialogFragment();
                errorFragment.setDialog(errorDialog);
                errorFragment.show(getSupportFragmentManager(), "Location Updates");
            }

            return false;
        }
    }

    @Override
    protected void onResume() {
        super.onResume();

        // Display the connection status

        if (mCurrentLocation != null) {
            Toast.makeText(this, "GPS location was found!", Toast.LENGTH_SHORT).show();
            LatLng latLng = new LatLng(mCurrentLocation.getLatitude(), mCurrentLocation.getLongitude());
            CameraUpdate cameraUpdate = CameraUpdateFactory.newLatLngZoom(latLng, 17);
            map.animateCamera(cameraUpdate);
        } else {
            Toast.makeText(this, "Current location was null, enable GPS on emulator!", Toast.LENGTH_SHORT).show();
        }
        MapDemoActivityPermissionsDispatcher.startLocationUpdatesWithPermissionCheck(this);
    }

    @NeedsPermission({Manifest.permission.ACCESS_FINE_LOCATION, Manifest.permission.ACCESS_COARSE_LOCATION})
    protected void startLocationUpdates() {
        mLocationRequest = new LocationRequest();
        mLocationRequest.setPriority(LocationRequest.PRIORITY_BALANCED_POWER_ACCURACY);
        mLocationRequest.setInterval(UPDATE_INTERVAL);
        mLocationRequest.setFastestInterval(FASTEST_INTERVAL);

        LocationSettingsRequest.Builder builder = new LocationSettingsRequest.Builder();
        builder.addLocationRequest(mLocationRequest);
        LocationSettingsRequest locationSettingsRequest = builder.build();

        SettingsClient settingsClient = LocationServices.getSettingsClient(this);
        settingsClient.checkLocationSettings(locationSettingsRequest);
        //noinspection MissingPermission
        getFusedLocationProviderClient(this).requestLocationUpdates(mLocationRequest, new LocationCallback() {
                    @Override
                    public void onLocationResult(LocationResult locationResult) {
                        onLocationChanged(locationResult.getLastLocation());
                    }
                },
                Looper.myLooper());
    }

    public void onLocationChanged(Location location) {
        // GPS may be turned off
        if (location == null) {
            return;
        }

        // Report to the UI that the location was updated

        mCurrentLocation = location;
        String msg = "Updated Location: " +
                Double.toString(location.getLatitude()) + "," +
                Double.toString(location.getLongitude());
        Toast.makeText(this, msg, Toast.LENGTH_SHORT).show();
    }

    public void onSaveInstanceState(Bundle savedInstanceState) {
        savedInstanceState.putParcelable(KEY_LOCATION, mCurrentLocation);
        super.onSaveInstanceState(savedInstanceState);
    }

    // Define a DialogFragment that displays the error dialog
    public static class ErrorDialogFragment extends android.support.v4.app.DialogFragment {

        // Global field to contain the error dialog
        private Dialog mDialog;

        // Default constructor. Sets the dialog field to null
        public ErrorDialogFragment() {
            super();
            mDialog = null;
        }

        // Set the dialog to display
        public void setDialog(Dialog dialog) {
            mDialog = dialog;
        }

        // Return a Dialog to the DialogFragment.
        @Override
        public Dialog onCreateDialog(Bundle savedInstanceState) {
            return mDialog;
        }
    }

}
