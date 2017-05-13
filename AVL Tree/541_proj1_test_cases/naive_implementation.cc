#include <vector>
#include <iostream>
#include <stdlib.h>

struct text_t{
    std::vector<char*> _text;
};


text_t * create_text(){
    return new text_t;
}


int length_text(text_t *txt){
    if (txt == NULL) return -1;
    return txt->_text.size();
}


char * get_line( text_t *txt, int index){
    return txt->_text[index-1];
}

void append_line( text_t *txt, char * new_line)
{
    txt->_text.push_back(new_line);
}


char * set_line( text_t *txt, int index, char * new_line){
    char *old = txt->_text[index-1];
    txt->_text[index-1] = new_line;
    return old;
}


void insert_line( text_t *txt, int index, char * new_line){
    txt->_text.insert(txt->_text.begin() + index - 1, new_line);
}

char * delete_line( text_t *txt, int index)
{
    char *old = txt->_text[index-1];
    txt->_text.erase(txt->_text.begin()+index-1);
    return old;
}