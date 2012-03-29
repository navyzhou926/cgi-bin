#!/bin/sh
    echo"Content-Type:text/plain"
    echo
    echo"*** Environment Variables ***"
    env
    echo"*** Standard Inputs "
    read buf
    echo $buf
