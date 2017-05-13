#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include "API.h"
struct Node {
	int key, other, min, max, leftmin, rightmax, height, measure;
	char which;
	struct Node *left;
	struct Node *right;
};
struct m_tree_t {
	struct Node *root;
};

int getHeight(struct Node *node) {
	return node ? node->height : 0;
}

int minimum(int a, int b, int c) {
	if (a < b) {
		if (a < c)
			return a;
		else
			return c;
	} else {
		if (c > b)
			return b;
		else
			return c;
	}
}

int maximum(int a, int b, int c) {
	if (a > b) {
		if (a > c)
			return a;
		else
			return c;
	} else {
		if (c < b)
			return b;
		else
			return c;
	}
}

void updateLeftmin(struct Node *node) {
	if (node->left == NULL && node->right == NULL) {
		if (node->which == 'l')
			node->leftmin = node->key;
		else
			node->leftmin = node->other;
	} else if (node->left == NULL && node->right != NULL) {
		node->leftmin =
				node->which == 'l' ?
						minimum(node->right->leftmin, node->key, INT_MAX) :
						minimum(node->right->leftmin, node->other, INT_MAX);
	} else if (node->left != NULL && node->right == NULL) {
		node->leftmin =
				node->which == 'l' ?
						minimum(node->left->leftmin, node->key, INT_MAX) :
						minimum(node->left->leftmin, node->other, INT_MAX);
	} else {
		node->leftmin =
				node->which == 'l' ?
						minimum(node->right->leftmin, node->key,
								node->left->leftmin) :
						minimum(node->right->leftmin, node->other,
								node->left->leftmin);
	}
}

void updateRightmax(struct Node *node) {
	if (node->left == NULL && node->right == NULL) {
		if (node->which == 'r')
			node->rightmax = node->key;
		else
			node->rightmax = node->other;
	} else if (node->left == NULL && node->right != NULL) {
		node->rightmax =
				node->which == 'r' ?
						maximum(node->right->rightmax, node->key, INT_MIN) :
						maximum(node->right->rightmax, node->other, INT_MIN);
	} else if (node->left != NULL && node->right == NULL) {
		node->rightmax =
				node->which == 'r' ?
						maximum(node->left->rightmax, node->key, INT_MIN) :
						maximum(node->left->rightmax, node->other, INT_MIN);
	} else {
		node->rightmax =
				node->which == 'r' ?
						maximum(node->right->rightmax, node->key,
								node->left->rightmax) :
						maximum(node->right->rightmax, node->other,
								node->left->rightmax);
	}
}

void updateMin(struct Node *node) {
	if (node->left == NULL)
		node->min = node->key;
	else {
		node->min = minimum(node->key, node->left->min, INT_MAX);
	}
}

void updateMax(struct Node *node) {
	if (node->right == NULL)
		node->max = node->key;
	else {
		node->max = maximum(node->key, node->right->max, INT_MIN);
	}
}

int search(struct Node *node, int x) {
	while (node != NULL) {
		if (node->key == x)
			return 1;
		else if (node->key > x)
			node = node->left;
		else
			node = node->right;
	}
	return 0;
}

void updateMeasure(struct Node *node) {
	if (node != NULL) {
		int leftMeasure = (node->left != NULL ? node->left->measure : 0);
		int rightMeasure = (node->right != NULL ? node->right->measure : 0);
		int rightMin = (node->right != NULL ? node->right->min : node->key);
		int rightMax = (node->right != NULL ? node->right->max : node->key);
		int leftMin = (node->left != NULL ? node->left->min : node->key);
		int leftMax = (node->left != NULL ? node->left->max : node->key);

		int LeftMinRightTree = (
				node->right != NULL ? node->right->leftmin : node->key);
		int RightMaxLeftTree = (
				node->left != NULL ? node->left->rightmax : node->key);

		int value = node->key;
		if (node->which == 'l') {
			if ((LeftMinRightTree == rightMin || LeftMinRightTree == value)
					&& RightMaxLeftTree == leftMax) {
				if (search(node->right, node->other)) {
					node->measure = leftMeasure + rightMeasure + rightMin
							- value;
				} else {
					node->measure = leftMeasure + rightMax - value;
				}
			} else if (RightMaxLeftTree != leftMax) {
				if (LeftMinRightTree < rightMin
						&& LeftMinRightTree == leftMin) {
					node->measure = rightMax - leftMin;
				} else {
					if (search(node->right, node->other)) {
						node->measure = leftMeasure + rightMeasure + rightMin
								- leftMax;
					} else {
						node->measure = leftMeasure + rightMax - leftMax;
					}
				}
			} else if ((LeftMinRightTree != rightMin
					&& LeftMinRightTree != value)
					&& RightMaxLeftTree == leftMax) {
				if (LeftMinRightTree <= leftMin) {
					node->measure = rightMax - leftMin;
				} else {
					if (search(node->right, node->other)) {
						node->measure = leftMeasure + rightMeasure + rightMin
								- node->min;
					} else {
						node->measure = rightMeasure + rightMin - leftMin;
					}
				}
			}
		} else {
			if ((RightMaxLeftTree == leftMax || RightMaxLeftTree == value)
					&& LeftMinRightTree == rightMin) {
				if (search(node->left, node->other)) {
					node->measure = leftMeasure + rightMeasure - leftMax
							+ value;
				} else {
					node->measure = rightMeasure - leftMin + value;
				}
			} else if (LeftMinRightTree != rightMin) {
				if (RightMaxLeftTree > leftMax
						&& RightMaxLeftTree == rightMax) {
					node->measure = rightMax - leftMin;
				} else {
					if (search(node->left, node->other)) {
						node->measure = leftMeasure + rightMeasure + rightMin
								- leftMax;
					} else {
						node->measure = rightMeasure + rightMin - leftMin;
					}
				}
			} else if ((RightMaxLeftTree != leftMax && RightMaxLeftTree != value)
					&& LeftMinRightTree == rightMin) {
				if (RightMaxLeftTree >= rightMax) {
					node->measure = rightMax - leftMin;
				} else {
					if (search(node->left, node->other)) {
						node->measure = leftMeasure + rightMeasure - leftMax
								+ node->max;
					} else {
						node->measure = leftMeasure + rightMax - leftMax;
					}
				}
			}
		}
	}
}

struct Node* createNode(int value, int other, char which) {
	struct Node* node = (struct Node*) malloc(sizeof(struct Node));
	node->key = value;
	node->height = 1;
	node->left = NULL;
	node->right = NULL;
	node->other = other;
	node->which = which;
	node->min = value;
	node->max = value;
	node->measure = 0;
	if (which == 'l')
		node->leftmin = value;
	else
		node->leftmin = other;
	if (which == 'r')
		node->rightmax = value;
	else
		node->rightmax = other;

	return node;
}
int fetchBalance(struct Node *node) {
	return node ? getHeight(node->left) - getHeight(node->right) : 0;
}

struct m_tree_t* create_m_tree() {
	struct m_tree_t *tree = (struct m_tree_t*) malloc(sizeof(struct m_tree_t));
	tree->root = NULL;
	return tree;
}

struct Node *rightRotate(struct Node *n) {
	struct Node *temp = n->left;
	struct Node *tree2 = temp->right;
	temp->right = n;
	n->left = tree2;
	n->height =
			getHeight(n->left) > getHeight(n->right) ?
					getHeight(n->left) : getHeight(n->right) + 1;
	temp->height =
			getHeight(temp->left) > getHeight(temp->right) ?
					getHeight(temp->left) : getHeight(temp->right) + 1;

	if (temp->left != NULL) {
		updateMin(temp->left);
		updateMax(temp->left);
		updateLeftmin(temp->left);
		updateRightmax(temp->left);
		updateMeasure(temp->left);
	}
	if (temp->right != NULL) {
		updateMin(temp->right);
		updateMax(temp->right);
		updateLeftmin(temp->right);
		updateRightmax(temp->right);
		updateMeasure(temp->right);
	}

	updateMin(temp);
	updateMax(temp);
	updateLeftmin(temp);
	updateRightmax(temp);
	updateMeasure(temp);
	return temp;
}

struct Node *leftRotate(struct Node *n) {
	struct Node *temp = n->right;
	struct Node *tree2 = temp->left;
	temp->left = n;
	n->right = tree2;
	n->height =
			getHeight(n->left) > getHeight(n->right) ?
					getHeight(n->left) : getHeight(n->right) + 1;
	temp->height =
			getHeight(temp->left) > getHeight(temp->right) ?
					getHeight(temp->left) : getHeight(temp->right) + 1;

	if (temp->left != NULL) {
		updateMin(temp->left);
		updateMax(temp->left);
		updateLeftmin(temp->left);
		updateRightmax(temp->left);
		updateMeasure(temp->left);
	}
	if (temp->right != NULL) {
		updateMin(temp->right);
		updateMax(temp->right);
		updateLeftmin(temp->right);
		updateRightmax(temp->right);
		updateMeasure(temp->right);
	}
	updateMin(temp);
	updateMax(temp);
	updateLeftmin(temp);
	updateRightmax(temp);
	updateMeasure(temp);
	return temp;
}

struct Node* insert(struct Node* node, int value, int other, char which) {
	if (!node)
		return createNode(value, other, which);
	if (value < node->key)
		node->left = insert(node->left, value, other, which);
	else if (value > node->key)
		node->right = insert(node->right, value, other, which);
	else {
		if (node->other <= other) {
			node->right = insert(node->right, value, other, which);
		} else {
			node->left = insert(node->left, value, other, which);
		}
	}

	node->height = 1
			+ (getHeight(node->left) > getHeight(node->right) ?
					getHeight(node->left) : getHeight(node->right));
	updateMin(node);
	updateMax(node);
	updateLeftmin(node);
	updateRightmax(node);
	updateMeasure(node);

	int hDiff = fetchBalance(node);

	if (hDiff > 1 && fetchBalance(node->left) >= 0) {
		return rightRotate(node);
	}

	if (hDiff > 1 && fetchBalance(node->left) < 0) {
		node->left = leftRotate(node->left);
		return rightRotate(node);
	}

	if (hDiff < -1 && fetchBalance(node->right) <= 0) {
		return leftRotate(node);
	}

	if (hDiff < -1 && fetchBalance(node->right) > 0) {
		node->right = rightRotate(node->right);
		return leftRotate(node);
	}

	return node;
}
struct Node* inorderNode(struct Node* node) {
	struct Node* temp = node;
	while (temp->left != NULL)
		temp = temp->left;
	return temp;
}
struct Node* removeNode(struct Node* node, int value, int other, char which) {
	if (!node)
		return node;

	if (value < node->key)
		node->left = removeNode(node->left, value, other, which);

	else if (value > node->key) {
		node->right = removeNode(node->right, value, other, which);
	}

	else {
		if (other != node->other) {
			if (node->other <= other)
				node->right = removeNode(node->right, value, other, which);
			else {
				node->left = removeNode(node->left, value, other, which);
			}
		} else {
			if ((!node->left) || (!node->right)) {
				struct Node *t = node->left ? node->left : node->right;
				if (!t) {
					t = node;
					node = NULL;
				} else {
					*node = *t;
				}
				free(t);
			} else {
				struct Node* temp = inorderNode(node->right);
				node->key = temp->key;
				node->other = temp->other;
				node->min = temp->min;
				node->max = temp->max;
				node->leftmin = temp->leftmin;
				node->rightmax = temp->rightmax;
				node->measure = temp->measure;
				node->height = temp->height;
				node->which = temp->which;
				node->right = removeNode(node->right, temp->key, temp->other,
						temp->which);
			}
		}
	}

	if (!node)
		return node;

	node->height = 1
			+ (getHeight(node->left) > getHeight(node->right) ?
					getHeight(node->left) : getHeight(node->right));

	updateMin(node);
	updateMax(node);
	updateLeftmin(node);
	updateRightmax(node);
	updateMeasure(node);
	int hDiff = fetchBalance(node);

	if (hDiff > 1 && fetchBalance(node->left) >= 0)
		return rightRotate(node);

	if (hDiff > 1 && fetchBalance(node->left) < 0) {
		node->left = leftRotate(node->left);
		return rightRotate(node);
	}

	if (hDiff < -1 && fetchBalance(node->right) <= 0)
		return leftRotate(node);

	if (hDiff < -1 && fetchBalance(node->right) > 0) {
		node->right = rightRotate(node->right);
		return leftRotate(node);
	}

	return node;
}
void postOrder(struct Node *root) {
	if (root != NULL) {
		postOrder(root->left);
		postOrder(root->right);
		free(root);
	}
}

void insert_interval(struct m_tree_t *tree, int value, int other) {
	if (value < other) {
		tree->root = insert(tree->root, value, other, 'l');
		tree->root = insert(tree->root, other, value, 'r');
	}
}
void delete_interval(struct m_tree_t* tree, int value, int other) {
	if (value < other) {
		tree->root = removeNode(tree->root, value, other, 'l');
		tree->root = removeNode(tree->root, other, value, 'r');
	}
}

void destroy_m_tree(struct m_tree_t* tree) {
	postOrder(tree->root);
	free(tree);
}
int query_length(struct m_tree_t *node) {
	if(node == NULL || node->root == NULL)
			return 0;
		return node->root->measure;
}
