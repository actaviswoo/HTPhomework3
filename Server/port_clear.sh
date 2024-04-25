#!/bin/bash

PID=$(sudo lsof -t -i :12345)

if [ -z "$PID" ]; then
    echo ""
else
    sudo kill -9 $PID
fi