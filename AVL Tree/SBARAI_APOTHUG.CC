#include<iostream>
#include<stdio.h>
#include "API.h"
using namespace std;
class text_t
{
public:
	int key;
	int height;
	text_t* left;
	text_t* right;
};
class Stack
{
	int top;
	int maxsize;
	text_t** s;
public:
	Stack()
	{
		maxsize=1000;
		s=new text_t*[maxsize];
		top=-1;
	}
	void push(text_t*node)
	{
		if(top==maxsize)
		{
			cout<<"Stack is full";
			return;
		}
		s[++top]=node;
	}
	bool isEmpty()
	{
		return top==-1;
	}
	text_t* pop()
	{
		text_t* x;
		if(isEmpty()==0)
		{
			x=s[top--];
		}
		else
		{
			cout<<"Stack is empty";
		}
		return x;
	}
	void deletestack(){
		top=-1;
		delete []s;
	}
};
int height(text_t *txt)
{
	if(txt==NULL)
	{
		return 0;
	}
	int h= 0;
	int lh = txt->left->height;
	int rh = txt->right->height;
	if(lh>rh)
	{
		h=lh+1;
	}
	else
	{
		h=rh+1;
	}
	return h;
}
int length_text(text_t *txt)
{
	int len=0;
	if(txt==NULL)
	{
		return -1;
	}
	if(txt->left==NULL)
	{
		return 0;
	}
	if(txt->right==NULL&&((char *)txt->left->left=="#"))
	{
		return 0;
	}
	else
	{
		if(txt->right==NULL)
		{
			return 0;
		}
		if(txt->right->left==NULL)
		{
			return 1;
		}
		else
		{
			while(txt->right!=NULL)
			{
				len=len+txt->key;
				txt=txt->right;
			}
			if(txt->key==0)
			{
				len=len+1;
			}
		}
		return len-1;
	}
}
void leftRotation(text_t *line)
{
	text_t *temp;
	int tempkey=line->key;
	line->key = line->key+line->right->key;
	temp = line->left;
	line->left = line->right;
	line->right = line->left->right;
	line->left->right = line->left->left;
	line->left->left = temp;
	line->left->key = tempkey;
}
void rightRotation(text_t *line)
{
	text_t *temp;
	int tempkey=line->key;
	line->key = line->left->key;
	temp = line->right;
	line->right = line->left;
	line->left = line->right->left;
	line->right->left = line->right->right;
	line->right->right = temp;
	line->right->key = tempkey-line->key;
}
void insert_line(text_t *txt,int index,char * new_line)
{
	if(txt==NULL)
	{
		return;
	}
	else if(txt->left ==NULL)
	{
		if(index>length_text(txt)+1)
		{
			index=length_text(txt)+1;
		}
		txt->left = (text_t *) new_line;
		txt->right = NULL;
		txt->key = 0;
		txt->height = 0;
	}
	else
	{
		Stack s;
		int last_flag =0;
		if(index>length_text(txt)+1)
		{
			index=length_text(txt)+1;
		}
		while(txt->right!=NULL)
		{
			s.push(txt);
			if(index>txt->key)
			{
				index=index-txt->key;
				txt=txt->right;
			}
			else if(index<=txt->key)
			{
				txt->key=txt->key+1;
				txt=txt->left;
			}
		}

		text_t * oline=new text_t();
		oline->key=0;
		oline->height=0;
		oline->left=txt->left;
		oline->right=txt->right;

		text_t * nline=new text_t();
		nline->key=0;
		nline->height=0;
		nline->left=(text_t *)new_line;
		nline->right=NULL;

		if(!(index-txt->key==0||index-txt->key==1))
		{
			txt->left=oline;
			txt->right=nline;
			txt->height=height(txt);
		}
		else
		{
			if(index-txt->key==0)
			{
				txt->left=nline;
				txt->right=oline;
				txt->height=height(txt);
			}
			else
			{
				txt->left=nline;
				txt->right=oline;
				txt->height=height(txt);
			}
		}

		txt->key=1;
		text_t * templine=NULL;
		while(s.isEmpty()==0 && last_flag==0)
		{
			int tempheight;
			templine = s.pop();
			int oheight= templine->height;
			if(templine->left->height - templine->right->height == -2)
			{
				if(templine->right->right->height-templine->left->height != 1)
				{
					rightRotation(templine->right);
					leftRotation(templine);
					tempheight = templine->right->right->height;
					templine->left->height = tempheight + 1;
					templine->right->height = tempheight + 1;
					templine->height = tempheight + 2;
				}
				else
				{
					leftRotation(templine);
					templine->left->height =templine->left->right->height + 1;
					templine->height = templine->left->height + 1;
				}
			}
			else if(templine->left->height - templine->right->height == 2)
			{
				if( templine->left->left->height - templine->right->height!= 1)
				{
					leftRotation(templine->left);
					rightRotation(templine);
					tempheight = templine->left->left->height;
					templine->left->height = tempheight + 1;
					templine->right->height = tempheight + 1;
					templine->height = tempheight + 2;
				}
				else
				{
					rightRotation(templine);
					templine->right->height =  templine->right->left->height + 1;
					templine->height = templine->right->height + 1;
				}
			}
			else
			{
				if(templine->left->height<=templine->right->height)
				{
					templine->height =templine->right->height + 1;
				}
				else
				{
					templine->height = templine->left->height + 1;
				}
			}
			if(templine->height == oheight)
				last_flag = 1;
		}
		s.deletestack();
	}
}
void append_line(text_t *txt, char * new_line)
{
	if(txt!=NULL)
	{
		insert_line(txt,length_text(txt)+1,new_line);
	}
	else
	{
		return;
	}
}
char * delete_line(text_t *txt, int index)
{
	if(txt==NULL)
	{
		return NULL;
	}
	else
	{
		int length=length_text(txt);
		if(index<=length)
		{
			text_t * topline,* selectline,* otherline;
			Stack st;
			int last_flag=0;
			while(txt->right!=NULL)
			{
				st.push(txt);
				topline=txt;
				if(index>txt->key)
				{
					index=index-txt->key;
					otherline=txt->left;
					selectline=txt->right;
					txt=txt->right;
				}
				else
				{
					otherline=txt->right;
					selectline=txt->left;
					txt->key=txt->key-1;
					txt=txt->left;
				}
			}
			st.pop();
			topline->key=otherline->key;
			topline->height=otherline->height;
			topline->left=otherline->left;
			topline->right=otherline->right;

			text_t * templine=NULL;
			while(st.isEmpty()==0 && last_flag==0)
			{
				int tempheight;
				templine = st.pop();
				int oheight= templine->height;

				if(templine->left->height-templine->right->height == -2)
				{
					if(templine->right->right->height-templine->left->height != 1)
					{
						rightRotation(templine->right);
						leftRotation(templine);
						tempheight = templine->right->right->height;
						templine->left->height = tempheight + 1;
						templine->right->height = tempheight + 1;
						templine->height = tempheight + 2;
					}
					else
					{
						leftRotation(templine);
						templine->left->height =templine->left->right->height + 1;
						templine->height = templine->left->height + 1;
					}
				}
				else if(templine->left->height - templine->right->height == 2)
				{
					if( templine->left->left->height - templine->right->height != 1 )
					{
						leftRotation(templine->left);
						rightRotation(templine);
						tempheight =templine->left->left->height;
						templine->left->height = tempheight + 1;
						templine->right->height = tempheight + 1;
						templine->height = tempheight + 2;
					}
					else
					{
						rightRotation(templine);
						templine->right->height =  templine->right->left->height + 1;
						templine->height = templine->right->height + 1;
					}
				}
				else
				{
					if(templine->left->height<=templine->right->height)
					{
						templine->height =	templine->right->height + 1;
					}
					else
					{
						templine->height = templine->left->height + 1;
					}
				}
				if(templine->height == oheight)
				{
					last_flag = 1;
				}
			}
			st.deletestack();
			return (char *)selectline->left;
		}
		else
		{
			return NULL;
		}
	}
}
text_t* create_text()
{
	text_t * node=new text_t();
	node->key=0;
	node->height=0;
	node->left=NULL;
	node->right=NULL;
	insert_line(node,1,"#");
	return node;
}

char * set_line(text_t *txt, int index, char * new_line)
{
	if(txt!=NULL)
	{
		while(txt->right!=NULL)
		{
			if(index>txt->key)
			{
				index=index-txt->key;
				txt=txt->right;
			}
			else if(index<=txt->key)
			{
				txt=txt->left;
			}
		}
		if(index==1)
		{
			char * t = (char*)txt->left;
			txt->left=(text_t *)new_line;
			return t;
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		return NULL;
	}
}
char * get_line(text_t *txt, int index)
{
	if(txt==NULL || index==0 || (index==length_text(txt)+1) || (index>length_text(txt)+1))
	{
		return NULL;
	}
	else
	{
		while(txt->right!=NULL)
		{
			if(index>txt->key)
			{
				index=index-txt->key;
				txt=txt->right;
			}
			else if(index<=txt->key)
			{
				txt=txt->left;
			}
		}
		return (char*)txt->left;
	}
}