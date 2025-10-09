ROOT_DIR := $(realpath ../../..)

INCLUDE_DIR := $(ROOT_DIR)/include

CC := gcc-14
CFLAGS := -Wall -Wextra -I$(INCLUDE_DIR) -std=c23
