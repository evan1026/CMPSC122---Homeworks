// Variable Tree Implementation File
// This is a simple binary search tree associating variables
// with integer values.
#include <iostream>
#include <string>
using namespace std;

#include "vartree.h"

//  recursiveSearch
//  A recursive tree-traversal function to search for a variable.
//  If the variable does not exist, it is created with a value of 0.
//  Parameters:
//      node    (modified TreeNode ptr) root of sub-tree to search
//      name    (input string)          name of variable
//  NOTE: node May be a null pointer, indicating an empty tree
//  Returns:
//      (TreeNode ptr) pointer to the found/created node with the given name
TreeNode* VarTree::recursiveSearch( TreeNode *&node, string name )
{
    if (node == NULL)
    {
        node = new TreeNode(name, 0);

        if (root == NULL)
            root = node;

        return node;
    }
    else if (name < node->name)
    {
        return recursiveSearch(node->left, name);
    }
    else if (name > node->name)
    {
        return recursiveSearch(node->right, name);
    }

    //If we didn't match an if statement up there, name == node->name, so node doesn't need to change

    return node;        
}

//  lookup
//  Searches for a variable to get its value
//  If the variable does not yet exist, it is created with value 0.
//  Parameters:
//      name (input char array) name of variable
//  Returns:  value of variable
int VarTree::lookup( string name )
{
    TreeNode *node = recursiveSearch( root, name );
    return node->value;
}

//  assign
//  Assigns a value to a variable.
//  If the variable does not yet exist, it is created.
//  Parameters:
//      name  (input string)  name of variable
//      value (input integer) value to assign
void VarTree::assign( string name, int value )
{
    TreeNode *node = recursiveSearch( root, name );
    node->value = value;
}

//  EXTRA CREDIT:  Implement the following, without any loops
ostream& operator<<( ostream& stream, VarTree &vt )
{
    stream << "{ ";

    if (vt.root != NULL)
        stream << *vt.root;

    stream << "}";

    return stream;
}

// What's cool about this implementation is that is will always be in alphabetical order
// simply because of the way the tree is structured
// Unfortunately, it's only alphabetical among variables of the same case
// All of the uppercase letters are less than the lowercase ones (because ASCII)
// So the variables starting with an uppercase letter will come before the ones with a
// lowercase letter
ostream& operator<<( ostream& stream, TreeNode &tn )
{
    if (tn.left != NULL)
        stream << *tn.left;

    stream << tn.name << "=" << tn.value << " ";

    if (tn.right != NULL)
        stream << *tn.right;

    return stream;
}
