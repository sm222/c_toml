#include "toml_utils.h"
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "gnl.h"


/// @brief 'invisible' form the user point of view
struct tomlFileEdit {
  char**            rawData;
  size_t            fileSize; // byte size
  size_t            totalLine;
  char*             fileName; // file name with out path
  char*             filePath; // file path;
  // - - - - - - - - - -
  size_t            line;     // reading line (line)
  size_t            cursor; // line + byte (rawData[line][cursor])
  // - - - - - - - - - - - - -
  int               error;
  t_knowKey*        keysList;
  size_t            keysListSize;
};

//********************************/
//              Start            */
//********************************/

void* _toml_init_data_(const char* filePath) {
  struct tomlFileEdit* file = calloc(sizeof(struct tomlFileEdit), 1);
  if (!file)
    return NULL;
  file->error = 1;
  if (!filePath || !filePath[0])
    return file;
  file->error = 2;
  if (strcmp(strrchr(filePath, FILE_EXTENSION[0]), FILE_EXTENSION) != 0)
    return file;
  file->error = 3;
  if (access(filePath, F_OK | R_OK) != 0) {
    perror(filePath);
    return file;
  }
  _toml_set_name(file, filePath);
  _toml_read_file(file);
  return file;
}


void _toml_set_name(void* file, const char* filePath) {
  struct tomlFileEdit* data = file;
  data->filePath = strdup(filePath);
  data->fileName = strrchr(data->filePath, FILE_SEP);
  if (!data->fileName) {
    data->fileName = data->filePath;
    data->filePath = NULL;
  }
}

void _toml_read_file(void* file) {
  struct tomlFileEdit* data = file;
  const char* path = data->filePath ? data->filePath : data->fileName;
  const int fd = open(path , O_RDONLY);
  if (fd < 0) {
    perror(path);
    return ;
  }
  char* line = "line";
  char* total = NULL;
  data->error = 0;
  while (line) {
    line = get_next_line(fd);
    total = ft_strjoin(total, line);
    free(line);
  }
  close(fd);
  data->rawData = ft_split(total, FILE_LINE_SEP);
  free(total);
  if (!data->rawData) {
    data->error = 4;
    return ;
  }
  for (size_t i = 0; data->rawData[i]; i++) {
    size_t tmpLen = strlen(data->rawData[i]);
    if (tmpLen > 1 && data->rawData[i][tmpLen - 1] == VALID_NEXT_LINE) {
      data->rawData[i][tmpLen - 1] = 0;
      tmpLen--;
    }
    data->fileSize += tmpLen;
    data->totalLine++;
  }
}

//********************************/
//             debug             */
//********************************/

// call just after readline
void _toml_print_error_parsing(tomlFile* file) {
  if (!file || !file->line || !file->rawData)
    return ;
  char buff[file->cursor + 2];
  memset(buff, '~', file->cursor + 1);
  buff[file->cursor + 1] = 0;
  printf("%zu:\n%s\n", file->line, file->rawData[file->line - 1]);
  printf("%s%s^%s\n", TXT_RED, !file->cursor ? "" : buff, TXT_RESET);
}

// never call :(
// need to redo
void _toml_print_error(const int error, const char* msg, const char* line) {
  if (line) {
    printf("c_toml: error:%d %s\n%s|\n", error, msg, line);
  }
  else
    printf("c_toml: error:%d %s\n", error, msg);
}

//********************************/
//            parsing            */
//********************************/

/// @brief 
/// @param file 
/// @return retun -1 on bad input/line read else return number of space skip
ssize_t _toml_skip_spaces(tomlFile* file) {
  if (!file || !file->rawData || !(*file->rawData))
    return -1;
  if (file->line >= file->totalLine)
    return -1;
  size_t lineLen = strlen(file->rawData[file->line]);
  while (file->cursor < lineLen) {
    if (strchr(VALID_SPACE, file->rawData[file->line][file->cursor])){
      file->cursor++;
      continue ;
    }
    break ;
  }
  return file->cursor;
}

/*********************************/
/*              var              */
/*********************************/

t_table* _toml_make_default_table(const char* name) {
  t_table* t = calloc(sizeof(*t), 1);
  if (t)
    t->tableName = strdup(name);
  return t;
}

t_table* _toml_make_tables(const char* name, t_field* fields, size_t fieldAmount) {
  t_table* t = _toml_make_default_table(name);
  if (t) {
    t->fields = fields;
    t->fieldAmount = fieldAmount;
  }
  return t;
}

t_field* _toml_make_default_field(const char* key) {
  t_field* f = calloc(sizeof(*f), 1);
  if (f)
    f->key = strdup(key);
  return f;
}

t_field* _toml_make_field(const char* key, e_types type, void* value, size_t ammout) {
  t_field* f = _toml_make_default_field(key);
  if (f) {
    f->type = type;
    f->value = value;
    f->amount = ammout;
  }
  return f;
}

//********************************/
//              end              */
//********************************/

void _toml_free_file(void* file) {
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
}