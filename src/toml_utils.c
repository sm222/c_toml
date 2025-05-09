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
  size_t            cursor;   // line + byte (rawData[line][cursor])
  ssize_t           currentLen; // strlen of (line) after toml_readline -1 line is NULL
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
void _toml_print_error_parsing(tomlFile* file, size_t line) {
  if (!file || !file->line || !file->rawData)
    return ;
  if (line >= file->totalLine)
    return;
  char buff[strlen(file->rawData[line - 1]) + 2];
  memset(buff, '~', file->cursor);
  buff[file->cursor + 1] = 0;
  printf("%zu:\n%s\n", file->line, file->rawData[line - 1]);
  printf("%s%s^%s\n", TXT_RED, !file->cursor ? "" : buff, TXT_RESET);
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
  size_t lineLen = strlen(file->rawData[LINE_OF_SET(file->line)]);
  while (file->cursor < lineLen) {
    if (strchr(VALID_SPACE, file->rawData[LINE_OF_SET(file->line)][file->cursor])){
      _toml_add_to_read(file, 2, 1);
      continue ;
    }
    break ;
  }
  return file->cursor;
}

/*********************************/
/*              var              */
/*********************************/

/*
* 1 line
* 2 cursor
* 3 line & cursor
*/
int _toml_zero_read(void* file, int mode) {
  if (!file)
    return 1;
  struct tomlFileEdit* data = file;
  if (mode == 1 || mode == 3)
    data->line = 0;
  if (mode == 2 || mode == 3)
    data->cursor = 0;
  return 0;
}

/*
* 1 line
* 2 cursor
* 3 line & cursor
*/
int _toml_add_to_read(void* file, int mode, int ammout) {
  if (!file)
    return 1;
  struct tomlFileEdit* data = file;
  if (mode == 1 || mode == 3)
    data->line += ammout;
  if (mode == 2 || mode == 3)
    data->cursor += ammout;
  return 0;
}

void     _toml_set_readLine_len(void* file ,ssize_t len) {
  if (!file)
    return;
  struct tomlFileEdit* data = file;
  data->currentLen = len;
}


bool  _toml_clean_known_keys(void* file) {
  if (!file)
    return false;
  struct tomlFileEdit* data = file;
  if (data->keysListSize) {
    free(data->keysList);
  }
  return true;
}

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
  //todo add free of keys list
}