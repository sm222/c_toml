
#include "c_toml.h"
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#include "gnl.h"


struct tomlFileEdit {
  char**        rawData;
  size_t        fileSize; // byte size
  size_t        totalLine;
  char*         fileName; // file name with out path
  char*         filePath; // file path;
  // - - - - - - - - - -
  size_t        head;     // reading head (line)
  size_t        headByte; // head + byte (rawData[head][headbyte])
  // - - - - - - - - - - - - -
  int           error;
};

/// @brief 
/// @param filePath 
/// @return only time it return null is if calloc fail
tomlFile* toml_init(const char* filePath) {
  struct tomlFileEdit* file = NULL;

  file = calloc(sizeof(struct tomlFileEdit), 1);
  if (!file)
    return (NULL);
  file->error = 1;
  if (!filePath || !filePath[0]) {
    return (tomlFile*)file;
  }
  const int fd = open(filePath, O_RDONLY);
  if (fd < 0)
    return (tomlFile*)file;
  char* line = "line";
  char* total = NULL;
  file->error = 0;
  while (line) {
    line = get_next_line(fd);
    total = ft_strjoin(total, line);
    free(line);
  }
  close(fd);
  file->rawData = ft_split(total, FILE_LINE_SEP);
  free(total);
  for (size_t i = 0; file->rawData[i]; i++) {
    size_t tmpLen = strlen(file->rawData[i]);
    if (tmpLen > 1 && file->rawData[i][tmpLen - 1] == VALID_NEXT_LINE) {
      file->rawData[i][tmpLen - 1] = 0;
      tmpLen--;
    }
    file->fileSize += tmpLen;
    file->totalLine++;
  }
  file->filePath = strdup(filePath);
  file->fileName = strrchr(file->filePath, FILE_SEP);
  if (!file->fileName) {
    file->fileName = file->filePath;
    file->filePath = NULL;
  }
  return (tomlFile*)file;
}


int toml_end(tomlFile* file) {
  if (!file)
    return 1;
  struct tomlFileEdit* freeFile = (struct tomlFileEdit*)file;
  if (freeFile->rawData) {
    for (size_t i = 0; freeFile->rawData[i]; i++) {
      free(freeFile->rawData[i]);
    }
  }
  free(freeFile->rawData);
  if (!freeFile->filePath)
    free(freeFile->fileName);
  free(freeFile->filePath);
  free(freeFile);
  return 0;
}


void toml_info(const tomlFile* file) {
  if (!file)
    return ;
  const int size = 1000;
  char      str[size];
  snprintf(str, size -1,"name: %s\n\tpath: %s\n"   \
    "\tsize: %zu\n\ttotal line: %zu\n\terror %d\n" \
    "head: %zu\nheadByte: %zu\n", file->fileName,  \
    file->filePath, file->fileSize, file->totalLine, file->error, file->head, file->headByte);
  write(1, str, strlen(str));
}

// dose not alloc memorry
const char* toml_readline(tomlFile* file, ssize_t* size) {
  if (size)
    *size = -1;
  if (!file)
    return NULL;
  if (file->head >= file->totalLine) {
    return NULL;
  }
  file->headByte = 0;
  size_t len = strlen(file->rawData[file->head]);
  while (file->headByte < len) {
    if (strchr(VALID_SPACE, file->rawData[file->head][file->headByte])) {
      file->headByte++;
    }
    else if (file->rawData[file->head][file->headByte] == COMMENT) {
      file->head++;
      return toml_readline(file, size);
    }
    else
      break ;
  }
  if (size)
    *size = len;
  return file->rawData[file->head++];
}
