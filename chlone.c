/*
   Chlone is a simple utility to clone file permissions
   Released under the New BSD license
 
 
   Copyright (c) 2013, Yousef Ourabi
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

      * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
      * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
      * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

void help(void);
void preconditions(struct stat *source, char *spath, struct stat *dest, char *dpath);
void clone(struct stat *source, char *dpath);
void reg_file_check(struct stat *source, char *path);
void err_wrap(int status, char *description);
void get_stat_or_exit(char *path, struct stat *s); 

int main (int argc, char *argv[]) {

  // Specify source and destination or display help message.
  if (argc != 3) {
    help();
  }

  // Source and destination paths
  char *spath = argv[1];
  char *dpath = argv[2];

  // Source and destination stat structs
  struct stat s_info;
  struct stat d_info;

  get_stat_or_exit(spath, &s_info);
  get_stat_or_exit(dpath, &d_info);

  preconditions(&s_info, spath, &d_info, dpath);

  clone(&s_info, dpath);

  return EXIT_SUCCESS;
}

// Help/usage... 
void help(void) {
  printf("\nUsage: chlone <source-path> <destination-path>\n\n");
  exit(EXIT_FAILURE);
}

void preconditions(struct stat *source, char *spath, struct stat *dest, char *dpath) {
  // Check if source and destination are same file.
  if (source->st_ino == dest->st_ino) {
    printf("Source and Destination are same file... nothing to be done!\n");
    exit(EXIT_SUCCESS);
  }
  
  reg_file_check(source, spath);
  reg_file_check(dest, dpath);
}

// Make sure chlone is only called on regular files
// Not sockets, characeter devices...etc...etc
void reg_file_check(struct stat *file, char *path) {
  if (!(file->st_mode & S_IFREG)) {
    fprintf(stderr, "%s is not a regular file. chlone only applies to regular files\n", path);
    exit(EXIT_FAILURE);
  }
}

// populate passed-in stat struct with file at path 
void get_stat_or_exit(char *path, struct stat *s) {
  if ( stat(path, s) < 0) {
    fprintf(stderr, "Error opening %s :: %s\n", path, strerror( errno ) );
    exit(EXIT_FAILURE);
  } 
}

// Clone User, Owner and Mode, and "User Defined" flags if supported
void clone(struct stat *source, char *dpath) {
  err_wrap(chmod(dpath, source->st_mode), "while changing mode" );
  err_wrap(chown(dpath, source->st_uid, source->st_gid), "while changing ownership");

  // Does this os/fs support "user-defined" flags
  // via stat->st_flags
  #if defined(__APPLE__) || defined(FreeBSD)
    err_wrap(chflags(dpath, source->st_flags), "while changing user-defined flags");
  #endif
}

// Wrap common idiom of return < 0 is error with errno populated 
void err_wrap(int status, char *description) {
  if (status < 0) {
    fprintf(stderr, "Error %s %s \n", strerror( errno ), description );
    exit(EXIT_FAILURE);
  }
}
