# Copyright (C) Intel Corporation, 2003 - 2011 All Rights Reserved.

# determine which library version to use
include MakefileOSVersion
IMRLIB = imrsdk$(OS_VERSION)

APP = amtider
APP_SRC = $(APP).cpp error-cast.cpp

CC = g++

INCLUDES :=  -I./
#Check the contents of /usr/local/ssl. If there is a directory named lib64 instead of lib, change -L$(OPENSSL_DIR)/lib to -L$(OPENSSL_DIR)/lib64
LIB_FLAGS  := -L./ -L/usr/local/ssl/lib64 -l$(IMRLIB) -luuid -lpthread -lssl -lcrypto -g

# Sum it all up
CFLAGS = $(INCLUDES) $(LIB_FLAGS)

all: $(APP)

$(APP): $(APP_SRC)
	$(CC) $(APP_SRC) $(CFLAGS) -o $@

clean:
	rm -f $(APP)

