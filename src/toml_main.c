
#include "c_toml.h"
#include "toml_utils.h"
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include "gnl.h"


const char* const toml_version(void) {
  static char s[31];
  #ifdef VERSION
  snprintf(s, 30, "c_toml version:%d.%d.%d", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
  #else
  snprintf(s, 20, "c_toml version: UNDEFINE");
  #endif
  return s;
}


static void _toml_print_error(const int error, const char* msg, const char* line) {
  if (line) {
    printf("c_toml: error:%d %s\n%s|\n", error, msg, line);
  }
  else
    printf("c_toml: error:%d %s\n", error, msg);
}

int toml_zero_read(tomlFile* file) {
  if (file) {
    file->head = 0;
    file->headByte = 0;
    return 0;
  }
  return 1;
}

static void _toml_read_file(struct tomlFileEdit* file) {
  const char* path = file->filePath ? file->filePath : file->fileName;
  const int fd = open(path , O_RDONLY);
  if (fd < 0) {
    perror("open");
    return ;
  }
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
}

static void _toml_set_name(struct tomlFileEdit* file, const char* filePath) {
  file->filePath = strdup(filePath);
  file->fileName = strrchr(file->filePath, FILE_SEP);
  if (!file->fileName) {
    file->fileName = file->filePath;
    file->filePath = NULL;
  }
}


/// @brief 
/// @param filePath 
/// @return only time it return null is if calloc fail
tomlFile* toml_init(const char* filePath) {
  struct tomlFileEdit* file = calloc(sizeof(struct tomlFileEdit), 1);
  if (!file)
    return (NULL);
  file->error = 1;
  if (!filePath || !filePath[0])
    return (tomlFile*)file;
  file->error = 2;
  if (strcmp(strrchr(filePath, FILE_EXTENSION[0]), FILE_EXTENSION) != 0)
    return (tomlFile*)file;
  file->error = 3;
  const int fd = open(filePath, O_RDONLY);
  if (fd < 0)
    return (tomlFile*)file;
  _toml_set_name(file, filePath);
  _toml_read_file(file);
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
    } // use to skip '#' comment line
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


static ssize_t _toml_look_for_valid_name(const char* name, const size_t end) {
  int test = 0;
  for (int i = 0; i < VALID_VAR_NAME_SECSION; i++) {
    for (int j = VALID_VAR_NAME[i * 2]; j < VALID_VAR_NAME[i * 2 + 1] + 1; j++) {
      if (*name == j) {test = j;}
    }
  }
  if (!test)
    return -1;
  size_t i = 1;
  for (; i < end; i++) {
    if (name[i] == TABLE_SIMBOLE[TABLE_CLOSE]) {
        break ;
      }
    if (name[i] == COMMENT) {
      return -1;
    }
  }
  return i;
}


static int _toml_look_type(const char* line, const size_t end, int *type) {
  size_t i = 0;
  int error = 1;
  int table = 0;
  *type = None;
  while (line[i] == TABLE_SIMBOLE[TABLE_OPEN]) {
    if (table > 2) {
      return 1;
    }
    table++;
    i++;
  }
  const ssize_t parseName = _toml_look_for_valid_name(line + table, end - table);
  if (table < 3 && parseName != -1) {
    *type = table ? (Table + table - 1) : Variable;
    i += parseName;
    while (i < end) {
      if (line[i] == TABLE_SIMBOLE[TABLE_CLOSE])
        table--;
      else
        break;
      i++;
    }
    if (table == 0)
      error = 0;
  }
  return error;
}

int toml_get_value(const char* line, const size_t lineLen, struct Variable* var) {
  if (!line || !var) {
    return 1;
  }
  int type;
  bzero(var, sizeof(*var));
  size_t i = 0;
  for (; i < lineLen; i++) {
    if (strchr(VALID_SPACE, line[i]) == NULL)
    break ;
  }
  if (i == lineLen)
    return 1;
  if (_toml_look_type(line + i, lineLen - i, &type))
    return 1;
  return 0;
}

int toml_is_file_valid(tomlFile* file) {
  int error = 0;
  if (!file)
  return 1;
  if (file->fileSize > MAX_FILE_SIZE || file->totalLine < 1)
  return 2;
  struct Variable vars[MAX_VARIABLE + 1];
  toml_zero_read(file);
  ssize_t lineLen = 0;
  for (size_t i = 0; i < file->totalLine - 1; i++) {
    const char* line = toml_readline(file, &lineLen);
    const int r = toml_get_value(line, lineLen, vars);
    error += r;
    printf("%s|%s\n", r ? "⛔" : "✅", line);
  }
  return error;
}
