#!/bin/bash

FILE=logger.log     
if [ -f $FILE ]; then
   rm $FILE
fi