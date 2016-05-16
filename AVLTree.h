#pragma once

template<class K,class V>
struct AVLTreeNode{
	AVLTreeNode<K,V> *_left;
	AVLTreeNode<K,V> *_right;
	AVLTreeNode<K,V> *_parent;
	K _key;
	V _value;
	int _bf;

	AVLTreeNode(const K &key,const V &value)
		:_key(key)
		,_value(value)
		,_left(NULL)
		,_right(NULL)
		,_parent(NULL)
		,_bf(0)
	{}
};

template<class K,class V>
class AVLTree{
	typedef AVLTreeNode<K,V> Node;
public:
	AVLTree()
		:_root(NULL)
	{}

	bool Insert(const K &key,const V &value)
	{
		//1.如果树为空
		if(_root == NULL)
		{
			_root = new Node(key,value);
			return true;
		}
		//2.不为空,找到正确的位置插入
		Node *cur = _root;
		Node *parent = NULL;
		while(cur != NULL)
		{
			if(cur->_key > key)
			{
				parent = cur;
				cur = cur->_left;
			}
			else if(cur->_key < key)
			{
                parent = cur;
				cur = cur->_right;
			}
			else 
				return false;
		}
		cur = new Node(key,value);
		if(key > parent->_key)
		{
			parent->_right = cur;
			cur->_parent = parent;
		}
		else
		{
			parent->_left = cur;
			cur->_parent = parent;
		}
		//3.调整树
		bool isRotate = false;
		Node *ppNode = NULL;
		while(parent)
		{
			if(cur == parent->_left)
				parent->_bf--;
			else
				parent->_bf++;

			if(parent->_bf == 0)  //已经平衡
				break;
			else if(parent->_bf == -1 || parent->_bf == 1)
			{	
				cur = parent;
				parent = cur->_parent;
			}
			else           //2或-2
			{
				isRotate = true;
				ppNode = parent->_parent;
				if(parent->_bf == 2)
				{
					if(cur->_bf == 1)
						_RotateL(parent);
					else
						_RotateRL(parent);
				}
				else
				{
					if(cur->_bf == -1)
						_RotateR(parent);
					else
						_RotateLR(parent);
				}
				break;
			}
		}
		if(isRotate)
		{
			if(ppNode == NULL)
				_root = parent;
			else
			{
				if(parent->_key > ppNode->_key)
					ppNode->_right = parent;
				else
					ppNode->_left = parent;
			}
		}
	}
	bool Banlance()
	{
		return _Banlance(_root);
	}
protected:
	bool _Banlance(Node *root)
	{
		if(root == NULL)
			return true;
		int left = _Height(root->_left);
		int right = _Height(root->_right);
		int bf = abs(left - right);
		if(bf > 1)
			return false;
		if(bf != abs(root->_bf))
		{
			cout<<root->_key<<":平衡因子有问题"<<endl;
			return false;
		}
		return _Banlance(root->_left) && _Banlance(root->_right);
	}
	int _Height(Node *root)
	{
		if(root == NULL)
			return 0;
		int left = _Height(root->_left) + 1;
		int right = _Height(root->_right) + 1;
		return left > right ? left:right;
	}
	void _RotateL(Node *& parent)
	{
		Node *subR = parent->_right;
		Node *subRL = subR->_left;
		
		parent->_right = subRL;
		if(subRL != NULL)
			subRL->_parent = parent;

		subR->_left = parent;
		subR->_parent = parent->_parent;
		parent->_parent = subR;

		parent->_bf = subR->_bf = 0;

		parent = subR;

	}
	void _RotateR(Node *& parent)
	{
		Node *subL = parent->_left;
		Node *subLR = subL->_right;
		
		parent->_left = subLR;
		if(subLR != NULL)
			subLR->_parent = parent;

		subL->_right = parent;
		subL->_parent = parent->_parent;
		parent->_parent = subL;

		parent->_bf = subL->_bf = 0;

		parent = subL;
	}
	void _RotateRL(Node *& parent)
	{
		Node *subR = parent->_right;
		Node *subRL = subR->_left;
		
		//右单旋
		subR->_left = subRL->_right;
		if(subRL->_right != NULL)
			subRL->_right->_parent = subR->_parent;

		subRL->_parent = subR->_parent;
		subRL->_right = subR;
		subR->_parent = subRL;

		if(subRL->_bf == 0 || subRL->_bf == 1)
			subR->_bf = 0;
		else
			subR->_bf = 1;

		//左单旋
		parent->_right = subRL->_left;
		if(subRL->_left != NULL)
			subRL->_left->_parent = parent;

		subR->_parent = parent->_parent;
		subRL->_left = parent;
		parent->_parent = subRL;
		if(subRL->_bf == 0 || subRL->_bf == -1)
			parent->_bf = 0;
		else
			parent->_bf = -1;

		subRL->_bf = 0;
		parent = subRL;
	}
	void _RotateLR(Node *& parent)
	{
		Node *subL = parent->_left;
		Node *subLR = subL->_right;

		//左单旋
		subL->_right = subLR->_left;
		if(subLR->_left != NULL)
			subLR->_left->_parent = subL;

		subLR->_parent = subL->_parent;
		subLR->_left = subL;
		subL->_parent = subLR;

		if(subLR->_bf == 0 || subLR->_bf == -1)
			subL->_bf = 0;
		else
			subL->_bf = -1;

		//右单旋转
		parent->_left = subLR->_right;
		if(subLR->_right != NULL)
			subLR->_right->_parent = parent;

		subLR->_parent = parent->_parent;
		subLR->_right = parent;
		parent->_parent = subLR;

		if(subLR->_bf == 0 || subLR->_bf == 1)
			parent->_bf = 0;
		else
			parent->_bf = 1;

		subLR->_bf = 0;
		parent = subLR;
	}
private:
	Node *_root;
};
