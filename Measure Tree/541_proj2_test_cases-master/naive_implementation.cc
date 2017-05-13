#include <set>
#include <utility>
#include <limits.h>
#include <stdio.h>
#include "API.h"

using namespace std;

typedef pair<int, int> interval_t;

struct m_tree_t{
    set<interval_t> _intervals;
};


m_tree_t * create_m_tree(){
}

void destroy_m_tree(m_tree_t *tree)
{
}


void insert_interval(m_tree_t * tree, int a, int b)
{
}


void delete_interval(m_tree_t * tree, int a, int b)
{
}


int query_length(m_tree_t * tree)
{
}