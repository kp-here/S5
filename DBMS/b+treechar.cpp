// B+ TREES using alphabets [A-Z]
// Abhiram K P
// B190281CS
// Set A
#include<iostream>
using namespace std;


class BTreeNode
{
	char *keys; 
	int t;	 
	BTreeNode **C; 
	int n;	 
	bool leaf; 

public:

	BTreeNode(char _t, bool _leaf); 

	
	void print();

	
	BTreeNode *search(char k); 

	
	char findKey(char k);

	
	void insertNonFull(char k);

	
	void splitChild(int i, BTreeNode *y);

	
	void DELETE(char k);

	
	void DELETEFromLeaf(int idx);

	
	void DELETEFromNonLeaf(int idx);

	
	char getPred(int idx);

	
	char getSucc(int idx);

	
	void fill(int idx);

	
	
	void borrowFromPrev(int idx);

	
	
	void borrowFromNext(int idx);

	
	void merge(int idx);

	
	friend class BTree;
};

class BTree
{
	BTreeNode *root; 
	int t; 
public:

	
	BTree(char _t)
	{
		root = NULL;
		t = _t;
	}

	void print()
	{
		if (root != NULL) root->print();
	}

	
	BTreeNode* search(char k)
	{
		return (root == NULL)? NULL : root->search(k);
	}

	
	void insert(char k);

	
	void DELETE(char k);

};

BTreeNode::BTreeNode(char t1, bool leaf1)
{
	
	t = t1;
	leaf = leaf1;

	
	keys = new char[2*t-1];
	C = new BTreeNode *[2*t];

	
	n = 0;
}


char BTreeNode::findKey(char k)
{
	int idx=0;
	while (idx<n && keys[idx] < k)
		++idx;
	return idx;
}


void BTreeNode::DELETE(char k)
{
	int idx = findKey(k);

	// The key to be DELETEd is present in this node
	if (idx < n && keys[idx] == k)
	{

		
		if (leaf)
			DELETEFromLeaf(idx);
		else
			DELETEFromNonLeaf(idx);
		cout<<"Key with value "<<k<<" has been deleted"<<endl;
	}
	else
	{

		
		if (leaf)
		{
			cout << "ERROR\n";
			return;
		}

		
		bool flag = ( (idx==n)? true : false );

		
		if (C[idx]->n < t)
			fill(idx);

		
		if (flag && idx > n)
			C[idx-1]->DELETE(k);
		else
			C[idx]->DELETE(k);
	}
	return;
}


void BTreeNode::DELETEFromLeaf (int idx)
{

	// Move all the keys after the idx-th pos one place backward
	for (int i=idx+1; i<n; ++i)
		keys[i-1] = keys[i];

	// Reduce the count of keys
	n--;

	return;
}


void BTreeNode::DELETEFromNonLeaf(int idx)
{

	char k = keys[idx];

	
	if (C[idx]->n >= t)
	{
		int pred = getPred(idx);
		keys[idx] = pred;
		C[idx]->DELETE(pred);
	}

	
	else if (C[idx+1]->n >= t)
	{
		char succ = getSucc(idx);
		keys[idx] = succ;
		C[idx+1]->DELETE(succ);
	}

	
	else
	{
		merge(idx);
		C[idx]->DELETE(k);
	}
	return;
}


char BTreeNode::getPred(int idx)
{
	
	BTreeNode *cur=C[idx];
	while (!cur->leaf)
		cur = cur->C[cur->n];

	
	return cur->keys[cur->n-1];
}

char BTreeNode::getSucc(int idx)
{

	
	BTreeNode *cur = C[idx+1];
	while (!cur->leaf)
		cur = cur->C[0];

	
	return cur->keys[0];
}


void BTreeNode::fill(int idx)
{

	
	if (idx!=0 && C[idx-1]->n>=t)
		borrowFromPrev(idx);

	
	else if (idx!=n && C[idx+1]->n>=t)
		borrowFromNext(idx);

	
	else
	{
		if (idx != n)
			merge(idx);
		else
			merge(idx-1);
	}
	return;
}


void BTreeNode::borrowFromPrev(int idx)
{

	BTreeNode *child=C[idx];
	BTreeNode *sibling=C[idx-1];

	

	
	for (int i=child->n-1; i>=0; --i)
		child->keys[i+1] = child->keys[i];

	if (!child->leaf)
	{
		for(int i=child->n; i>=0; --i)
			child->C[i+1] = child->C[i];
	}

	
	child->keys[0] = keys[idx-1];

	
	if(!child->leaf)
		child->C[0] = sibling->C[sibling->n];

	
	keys[idx-1] = sibling->keys[sibling->n-1];

	child->n += 1;
	sibling->n -= 1;

	return;
}


void BTreeNode::borrowFromNext(int idx)
{

	BTreeNode *child=C[idx];
	BTreeNode *sibling=C[idx+1];

	// keys[idx] is inserted as the last key in C[idx]
	child->keys[(child->n)] = keys[idx];

	// Sibling's first child is inserted as the last child
	// into C[idx]
	if (!(child->leaf))
		child->C[(child->n)+1] = sibling->C[0];

	//The first key from sibling is inserted into keys[idx]
	keys[idx] = sibling->keys[0];

	// Moving all keys in sibling one step behind
	for (int i=1; i<sibling->n; ++i)
		sibling->keys[i-1] = sibling->keys[i];

	// Moving the child pointers one step behind
	if (!sibling->leaf)
	{
		for(int i=1; i<=sibling->n; ++i)
			sibling->C[i-1] = sibling->C[i];
	}

	
	child->n += 1;
	sibling->n -= 1;

	return;
}


void BTreeNode::merge(int idx)
{
	BTreeNode *child = C[idx];
	BTreeNode *sibling = C[idx+1];

	
	child->keys[t-1] = keys[idx];

	// Copying the keys from C[idx+1] to C[idx] at the end
	for (int i=0; i<sibling->n; ++i)
		child->keys[i+t] = sibling->keys[i];

	// Copying the child pointers from C[idx+1] to C[idx]
	if (!child->leaf)
	{
		for(int i=0; i<=sibling->n; ++i)
			child->C[i+t] = sibling->C[i];
	}

	
	for (int i=idx+1; i<n; ++i)
		keys[i-1] = keys[i];

	// Moving the child pointers after (idx+1) in the current node one
	// step before
	for (int i=idx+2; i<=n; ++i)
		C[i-1] = C[i];

	// Updating the key count of child and the current node
	child->n += sibling->n+1;
	n--;

	// Freeing the memory occupied by sibling
	delete(sibling);
	return;
}

// The main function that inserts a new key in this B-Tree
void BTree::insert(char k)
{
	// If tree is empty
	if (root == NULL)
	{
		// Allocate memory for root
		root = new BTreeNode(t, true);
		root->keys[0] = k; // Insert key
		root->n = 1; // Update number of keys in root
	}
	else // If tree is not empty
	{
		// If root is full, then tree grows in height
		if (root->n == 2*t-1)
		{
			// Allocate memory for new root
			BTreeNode *s = new BTreeNode(t, false);

			// Make old root as child of new root
			s->C[0] = root;

			// Split the old root and move 1 key to the new root
			s->splitChild(0, root);

			
			int i = 0;
			if (s->keys[0] < k)
				i++;
			s->C[i]->insertNonFull(k);

			// Change root
			root = s;
		}
		else 
			root->insertNonFull(k);
	}
}


void BTreeNode::insertNonFull(char k)
{
	// Initialize index as index of rightmost element
	int i = n-1;

	// If this is a leaf node
	if (leaf == true)
	{
		
		while (i >= 0 && keys[i] > k)
		{
			keys[i+1] = keys[i];
			i--;
		}

		// Insert the new key at found location
		keys[i+1] = k;
		n = n+1;
	}
	else // If this node is not leaf
	{
		// Find the child which is going to have the new key
		while (i >= 0 && keys[i] > k)
			i--;

		// See if the found child is full
		if (C[i+1]->n == 2*t-1)
		{
			// If the child is full, then split it
			splitChild(i+1, C[i+1]);

			
			if (keys[i+1] < k)
				i++;
		}
		C[i+1]->insertNonFull(k);
	}
}


void BTreeNode::splitChild(int i, BTreeNode *y)
{
	
	BTreeNode *z = new BTreeNode(y->t, y->leaf);
	z->n = t - 1;

	// Copy the last (t-1) keys of y to z
	for (int j = 0; j < t-1; j++)
		z->keys[j] = y->keys[j+t];

	
	if (y->leaf == false)
	{
		for (int j = 0; j < t; j++)
			z->C[j] = y->C[j+t];
	}

	
	y->n = t - 1;

	
	for (int j = n; j >= i+1; j--)
		C[j+1] = C[j];

	
	C[i+1] = z;

	
	for (int j = n-1; j >= i; j--)
		keys[j+1] = keys[j];

	
	keys[i] = y->keys[t-1];

	
	n = n + 1;
}


void BTreeNode::print()
{
	
	int i;
	for (i = 0; i < n; i++)
	{
		
		if (leaf == false)
			C[i]->print();
		cout << keys[i]<<" ";
		
	}

	// Print the subtree rooted with last child
	if (leaf == false)
		C[i]->print();
}


BTreeNode *BTreeNode::search(char k)
{
	
	int i = 0;
	while (i < n && k > keys[i])
		i++;

	
	if (keys[i] == k)
	{
		cout<<"TRUE"<<endl;
		return this;
		
	}
		
		
	else
	{
		cout<<"FALSE"<<endl;
	}

	
	if (leaf == true)
		return NULL;

	
	return C[i]->search(k);
}

void BTree::DELETE(char k)
{
	if (!root)
	{
		cout << "ERROR\n";
		return;
	}

	
	root->DELETE(k);

	
	if (root->n==0)
	{
		BTreeNode *tmp = root;
		if (root->leaf)
			root = NULL;
		else
			root = root->C[0];

		// Free the old root
		delete tmp;
	}
	return;
}


int main()
{
	BTree t(4); 

	char temp;
	char c;
	do
	{
		cin>>c;
		if(c=='e')
		{
			break;
		}
		if (c=='p')
		{
			t.print();
			cout<<endl;
		}
		else
		{
			cin>>temp;
            if ('A'<=temp && temp<='Z')
            {
                if (c=='i')
                {
                    t.insert(temp);
                }
                if (c=='d')
                {
                    t.DELETE(temp);
                }
                if (c=='s')
                {
                    t.search(temp);
                }
            }
            else
            {
                cout<<"Enter an alphabet (A-Z)"<<endl;
            }
			
		}
		
		
	}
	while(c!='e');

	
	return 0;
}
