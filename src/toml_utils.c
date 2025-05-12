#include "toml_utils.h"
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "gnl.h"


/// @brief 'invisible' form the user point of view
struct tomlFileEdit {
  char**            rawData;        //
  size_t            fileSize;       // byte size
  size_t            totalLine;      //
  char*             fileName;       // file name with out path
  char*             filePath;       // file path;
  // - - - - - - - - - - - - - - - -//
  size_t            line;           // reading line (line)
  size_t            cursor;         // line + byte (rawData[line][cursor])
  ssize_t           currentLineLen; // strlen of (line) after toml_readline -1 line is NULL
  // - - - - - - - - - - - - - - - -//
  int               error;          //
  t_knowKey*        keysList;       //
  size_t            keysListSize;   //
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

int  _toml_get_error(void* file) {
  if (!file)
    return -1;
  struct tomlFileEdit* data = (struct tomlFileEdit*)file;
  return data->error;
}

// call just after readline
void _toml_print_error_parsing(void* file) {
  if (!file)
    return ;
  struct tomlFileEdit* data = (struct tomlFileEdit*)file;
  if (!data->line || !data->rawData)
    return ;
  if (data->line - 1 >= data->totalLine)
    return;
  char buff[strlen(data->rawData[data->line - 1]) + 2];
  memset(buff, '~', data->cursor);
  buff[data->cursor + 1] = 0;
  printf("%zu:\n%s\n", data->line, data->rawData[data->line - 1]);
  printf("%s%s^%s\n", TXT_RED, !data->cursor ? "" : buff, TXT_RESET);
}


void  _toml_print_l(void* file) {
  if (!file)
    return;
  struct tomlFileEdit* data = (struct tomlFileEdit*)file;
  const char* l = data->rawData[LINE_OF_SET(data->line)];
  printf("%zu:%zu [%zu]\n%s\n%s\n", strlen(l), data->cursor, data->cursor ,l, data->cursor < (size_t)data->currentLineLen ? l : ":<- end");
  printf("____________________________\n");
}

void    _toml_info(const tomlFile file) {
  if (!file)
    return;
  struct tomlFileEdit* data = (struct tomlFileEdit*)file;
  const int size = 1001;
  char      str[size];
  snprintf(str, size -1,"name: %s\n\tpath: %s\n"  \
  "\tsize: %zu\n\ttotal line: %zu\n\terror %d\n"\
  "line: %zu\ncursor: %zu\n", data->fileName,   \
  data->filePath, data->fileSize, data->totalLine, data->error, data->line, data->cursor);
  write(1, str, strlen(str));
}

//********************************/
//            parsing            */
//********************************/

ssize_t     _toml_get_file_byte_size(tomlFile file) {
  if (!file)
    return -1;
  struct tomlFileEdit* data = (struct tomlFileEdit*)file;
  return data->fileSize;
}

ssize_t _toml_get_file_line_number(tomlFile file) {
  if (!file)
    return -1;
  struct tomlFileEdit* data = (struct tomlFileEdit*)file;
  return data->totalLine;
}



const char* _toml_read_line(tomlFile file, ssize_t* size) {
  if (!file)
    return NULL;
  _toml_zero_read(file, 2);
  struct tomlFileEdit* data = (struct tomlFileEdit*)file;
  if (data->line >= data->totalLine)
    return NULL;
  const size_t skip = _toml_skip_spaces(file);
  const size_t len = strlen(data->rawData[LINE_OF_SET(data->line)]);
  if (data->rawData[LINE_OF_SET(data->line)][skip] == COMMENT) {
    _toml_add_to_read(file, 1, 1);
    return _toml_read_line(file, size);
  }
  if (size)
    *size = len;
  _toml_set_readLine_len(file, len);
  return data->rawData[LINE_OF_SET(data->line)];
}

int _toml_get_name(tomlFile file) {
  const ssize_t space = _toml_skip_spaces(file);
  if (space == -1)
    return 1;
  struct tomlFileEdit* data = (struct tomlFileEdit*)file;
  int table = 0;
  _toml_add_to_read(file, 2, space);
  while (table < 3 && data->cursor < (size_t)data->currentLineLen) {
    _toml_add_to_read(file, 2, 1);
  }
  _toml_print_l(file);
  return 0;
}

/// @brief 
/// @param file 
/// @return retun -1 on bad input/line read else return number of space skip
ssize_t _toml_skip_spaces(void* file) {
  if (!file) 
    return -1;
  struct tomlFileEdit* data = (struct tomlFileEdit*)file;
  if (!data->rawData || !(*data->rawData))
    return -1;
  if (data->line >= data->totalLine)
    return -1;
  size_t spaceSkip = 0;
  while (data->cursor < (size_t)data->currentLineLen) {
    if (strchr(VALID_SPACE, data->rawData[LINE_OF_SET(data->line)][data->cursor])) {
      _toml_add_to_read(file, 2, 1);
      spaceSkip++;
      continue ;
    }
    break ;
  }
  return spaceSkip;
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
  data->currentLineLen = len;
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