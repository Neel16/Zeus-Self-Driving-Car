#!/bin/bash

dbc="243.dbc"
for module in ble gps master motor sensor
do
  cp $dbc $module/_can_dbc/$dbc
done
