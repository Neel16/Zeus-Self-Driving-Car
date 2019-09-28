package com.example.mapdemo;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.Button;
import android.view.View;
import android.content.Intent;
import android.content.IntentFilter;


public class StartScreen extends AppCompatActivity {

    private Button button2;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_start_screen);

        button2 = (Button) findViewById(R.id.button2);  //OPEN MAP
        button2.setOnClickListener(new View.OnClickListener() {  ////on click calls openmap()
            @Override
            public void onClick(View v) {
                openmap();
            }
        });


    }
    public void openmap(){
        Intent intent1 = new Intent(this,  MapDemoActivity.class);
        startActivity(intent1);
    }

}

