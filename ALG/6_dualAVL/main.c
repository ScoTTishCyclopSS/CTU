#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define I(x, k, d) (x + (k * d))
#define W(m, x, d) ((m - x) / d)

//#define PRINT_PROCESS 1
#undef PRINT_PROCESS

#define MAX(x, y) (((x) > (y)) ? (x) : (y))

#define SHIFTR(_arr, _from, _to)             \
    for (int _i = _to + 1; _i > _from; _i--) \
    {                                        \
        _arr[_i] = _arr[_i - 1];             \
    }
#define SHIFTL(_arr, _from, _to)             \
    for (int _i = _from - 1; _i < _to; _i++) \
    {                                        \
        _arr[_i] = _arr[_i + 1];             \
    }

typedef struct Node
{
    int keys[4], h, key_count;
    struct Node *left, *right, *parent;
} Node_t;

Node_t *min_node(Node_t *node);
int close_key(Node_t *node, int key);
Node_t *Delete(int key, Node_t *node);
Node_t *Insert(int key, Node_t *node, Node_t *parent);
Node_t *check_balance(Node_t *node, int key);
Node_t *rotation_left(Node_t *node);
Node_t *rotation_right(Node_t *node);
Node_t *NewNode(int key, Node_t *parent);
int present_key(Node_t *node, int key);
void add_key(Node_t *node, int key);
void remove_key(Node_t *node, int key_id);
int is_leaf(Node_t *node);
void split(Node_t *node, int key);
void treeprint(Node_t *root, int level, char c);

int test(Node_t *node);
int height_of(Node_t *node);
void stats(Node_t *node);

int nodes, leaf_1, leaf_2, leaf_3;

int main()
{
    int op;

    Node_t *root = NULL;

    if (scanf("%d", &op) != 1)
    {
        return 1;
    }

    for (int i = 0; i < op; i++)
    {
        int x, d, m;
        char c;

        if (scanf(" %c %d %d %d", &c, &x, &d, &m) != 4)
        {
            return 1;
        }

        if (c == 'I')
        {
            int w = W(m, x, d);
            for (int k = 0; k <= w; k++)
            {
#ifdef PRINT_PROCESS
                printf("----------------------I(%d)----------------------\n", I(x, k, d));
#endif

                root = Insert(I(x, k, d), root, NULL);
#ifdef PRINT_PROCESS
                printf("\n");
                treeprint(root, 0, 'o');
                printf("\n");
#endif
            }
        }

        if (c == 'D')
        {
            int w = W(m, x, d);
            for (int k = 0; k <= w; k++)
            {
#ifdef PRINT_PROCESS
                printf("----------------------D(%d)----------------------\n", I(x, k, d));
#endif
                root = Delete(I(x, k, d), root);
#ifdef PRINT_PROCESS
                printf("\n");
                treeprint(root, 0, 'o');
                printf("\n");
#endif
            }
        }
    }

    // printf("\n");
    // treeprint(root, 0, 'o');
    // printf("\n");

    stats(root);

    printf("%d %d %d %d\n", nodes, leaf_1, leaf_2, leaf_3);

    return 0;
}

void stats(Node_t *node)
{
    if (node)
    {
        nodes++;
    }
    else
    {
        return;
    }

    if (is_leaf(node))
    {
        if (node->key_count == 1)
        {
            leaf_1++;
        }
        else if (node->key_count == 2)
        {
            leaf_2++;
        }
        else if (node->key_count == 3)
        {
            leaf_3++;
        }
    }

    stats(node->right);
    stats(node->left);
}

Node_t *min_node(Node_t *node)
{
    Node_t *curr = node;
    while (curr->left)
        curr = curr->left;
    return curr;
}

int test(Node_t *node)
{
    int y;
    if (node->left)
    {
        y = test(node->left);
    }
    else
    {
        y = node->keys[0];
        node->parent = Delete(y, node->parent);
    }
    return y;
}

int close_key(Node_t *node, int key)
{
    int res = node->keys[0];

    for (int i = 0; i < node->key_count; i++)
    {
        if (abs(key - res) > abs(key - node->keys[i]))
        {
            res = node->keys[i];
        }
    }

    return res;
}

Node_t *Delete(int key, Node_t *node)
{
    if (!node)
        return node;

    int id = present_key(node, key);

    /*
     * The inside node
     */
    if (!is_leaf(node))
    {
        /*
         * Inside node doesn't have a key
         */
        if (id == -1)
        {
            // if both the brothers are leafs with only one key
            if (is_leaf(node->left) && is_leaf(node->right) && node->left->key_count == 1 && node->right->key_count == 1)
            {
                // 8. condition
                if (present_key(node->left, key) != -1)
                {
#ifdef PRINT_PROCESS
                    printf("%d -> [8.] My child has a key, it's the left one: %d\n", key, node->left->keys[0]);
#endif

                    add_key(node, node->right->keys[0]);
                    node->left = NULL;
                    node->right = NULL;
                    node->h = 0;
                }
                else if (present_key(node->right, key) != -1)
                {
#ifdef PRINT_PROCESS
                    printf("%d -> [8.] My child has a key, it's the right one: %d\n", key, node->right->keys[0]);
#endif

                    add_key(node, node->left->keys[0]);
                    node->left = NULL;
                    node->right = NULL;
                    node->h = 0;
                }
                node = check_balance(node, key);
            }
            else if (key < node->keys[0]) // k < min(U)
            {
                node->left = Delete(key, node->left); // delete(k, U.L)
            }
            else if (key > node->keys[1]) // k > max(U)
            {
                node->right = Delete(key, node->right); // delete(k, U.R)
            }
        }
        /*
         * Inside node does have a key
         */
        else
        {
// 4. condition
#ifdef PRINT_PROCESS
            printf("%d -> [4.] The key is in inside node\n", key);
#endif

            remove_key(node, id);                                                                           // remove k from U
            int y = height_of(node->left) <= height_of(node->right) ? test(node->right) : test(node->left); // find min in Ly

#ifdef PRINT_PROCESS
            printf("%d -> The min key is %d\n", key, y);
#endif

            add_key(node, y); // add y to U
            node->h = MAX(height_of(node->left), height_of(node->right)) + 1;
        }
        return node;
    }

    /*
     * The leaf cases
     */

    // simple key remove from node
    if (node->key_count > 1 && id != -1)
    {
// 5. condition
#ifdef PRINT_PROCESS
        printf("%d -> [5.] Remove from leaf\n", key);
#endif

        remove_key(node, id);
        return node;
    }

    Node_t *sibler = node->parent->left;
    if (sibler == node)
        sibler = node->parent->right;

    // node has one key && sibler is not leaf
    if (sibler)
    {
        if (node->key_count == 1 && !is_leaf(sibler) && id != -1)
        {
// 6. condition
#ifdef PRINT_PROCESS
            printf("%d -> [6.] leaf with 1 search key + inside sliber\n", key);
#endif

            int z = close_key(node->parent, key);
            node->keys[0] = z;
            node->parent = Delete(z, node->parent);
        }

        // node has only one key, when his leaf sibler more than one
        else if (node->key_count == 1 && is_leaf(sibler) && sibler->key_count > 1 && id != -1)
        {
// 7. condition
#ifdef PRINT_PROCESS
            printf("%d -> [7.] leaf with 1 search key + leaf sliber with > 1 keys\n", key);
#endif

            remove_key(node, id);
            // making the M arr

            Node_t *M = NewNode(sibler->keys[0], NULL);
            remove_key(sibler, 0);

            while (sibler->key_count != 0)
            {
                // filling M until empty sibler
                add_key(M, sibler->keys[0]);
                remove_key(sibler, 0);
            }

            while (node->parent->key_count != 0)
            {
                // filling M until empty parent
                add_key(M, node->parent->keys[0]);
                remove_key(node->parent, 0);
            }

#ifdef PRINT_PROCESS
            for (int i = 0; i < M->key_count; i++)
            {
                printf("%d ", M->keys[i]);
            }
            printf("\n");
#endif

            int n = 0, p = 0;

            while (M->key_count > 0)
            {
                int del_pos = node->parent->right == node ? M->key_count - 1 : 0;
                if (n != 1)
                {
                    add_key(node, M->keys[del_pos]);
                    n = 1;
                }
                else if (p != 2)
                {
                    add_key(node->parent, M->keys[del_pos]);
                    p++;
                }
                else
                {
                    add_key(sibler, M->keys[del_pos]);
                }
                remove_key(M, del_pos);
            }

            free(M);
        }
    }

    node = check_balance(node, key);
    return node;
}

int height_of(Node_t *node)
{
    return node ? node->h : 0;
}

Node_t *Insert(int key, Node_t *node, Node_t *parent)
{
    if (node == NULL)
    {
        return NewNode(key, parent);
    }
    /*
     * Key is already inside the tree
     */
    for (int i = 0; i < node->key_count; i++)
    {
        if (key == node->keys[i])
            return node;
    }

    /*
     * Leaf
     */
    if (is_leaf(node))
    {
        if (node->key_count < 3)
        {
            add_key(node, key);
        }
        else
        {
            split(node, key);
        }
        /*
         * Standard
         */
    }
    else
    {
        if (key < node->keys[0]) // k < min(U)
        {
            node->left = Insert(key, node->left, node);
        }
        else if (key > node->keys[1])
        {
            node->right = Insert(key, node->right, node); // k > max(U)
        }
        else
        {
#ifdef PRINT_PROCESS
            printf("key: %d -> The 3.3c rule on < ", key);
            for (int i = 0; i < node->key_count; i++)
            {
                printf("%d ", node->keys[i]);
            }
            printf(">\n");
#endif

            if (height_of(node->left) <= height_of(node->right)) // the 3.3c rule
            {
                node->left = Insert(node->keys[0], node->left, node); // Insert(min(U), U.L)
                node->keys[0] = key;                                  // min(U) = k
            }
            else
            {
                node->right = Insert(node->keys[1], node->right, node); // Insert(max(U), U.R)
                node->keys[1] = key;                                    // max(U) = k
            }
        }
        node->h = MAX(height_of(node->left), height_of(node->right)) + 1;
    }
    return check_balance(node, key);
}

Node_t *check_balance(Node_t *node, int key)
{
    int balance = height_of(node->left) - height_of(node->right);

#ifdef PRINT_PROCESS
    printf("%d -> Checking balance on < ", key);
    for (int i = 0; i < node->key_count; i++)
    {
        printf("%d ", node->keys[i]);
    }
    printf(">\n");
#endif

    if (balance < -1)
    {
#ifdef PRINT_PROCESS
        printf("key: %d -> The balance is unbalanced to right!\n", key);
#endif

        if (key >= node->right->keys[1])
        {
#ifdef PRINT_PROCESS
            printf("key: %d -> The single left rotation!\n", key);
#endif

            node = rotation_left(node);
        }
        else
        {
#ifdef PRINT_PROCESS
            printf("key: %d -> The double left rotation!\n", key);
#endif

            node->right = rotation_right(node->right);
            node = rotation_left(node);
        }
    }
    else if (balance > 1)
    {
#ifdef PRINT_PROCESS
        printf("key: %d -> The balance is unbalanced to left!\n", key);
#endif

        if (key <= node->left->keys[0])
        {
#ifdef PRINT_PROCESS
            printf("key: %d -> The single right rotation!\n", key);
#endif

            node = rotation_right(node);
        }
        else
        {
#ifdef PRINT_PROCESS
            printf("key: %d -> The double right rotation!\n", key);
#endif

            node->left = rotation_left(node->left);
            node = rotation_right(node);
        }
    }
    return node;
}

Node_t *rotation_left(Node_t *node)
{
    Node_t *node_p = node->parent;

    Node_t *n_r = node->right; // take the right node
    Node_t *T2 = n_r->left;

    T2->parent = node;

    n_r->left = node;
    node->right = T2;

    node->h = MAX(height_of(node->left), height_of(node->right)) + 1;
    n_r->h = MAX(height_of(n_r->left), height_of(n_r->right)) + 1;

    n_r->parent = node_p;
    node->parent = n_r;

    return n_r;
}

Node_t *rotation_right(Node_t *node)
{
    Node_t *node_p = node->parent;

    Node_t *n_l = node->left;
    Node_t *T2 = n_l->right;

    T2->parent = node;

    n_l->right = node;
    node->left = T2;

    node->h = MAX(height_of(node->left), height_of(node->right)) + 1;
    n_l->h = MAX(height_of(n_l->left), height_of(n_l->right)) + 1;

    n_l->parent = node_p;
    node->parent = n_l;

    return n_l;
}

Node_t *NewNode(int key, Node_t *parent)
{
#ifdef PRINT_PROCESS
    printf("key: %d -> Creating new Node!\n", key);
#endif

    Node_t *node = malloc(sizeof(Node_t));
    node->h = 0;
    node->left = node->right = NULL;
    node->parent = parent;
    node->keys[0] = key;
    node->key_count = 1;
    return node;
}

int present_key(Node_t *node, int key)
{
    for (int i = 0; i < node->key_count; i++)
    {
        if (node->keys[i] == key)
            return i;
    }
    return -1;
}

void add_key(Node_t *node, int key)
{
    if (node->key_count == 0)
    {
        node->keys[0] = key;
    }

    else
    {
        for (int i = 0; i < node->key_count; i++)
        {
            if (key < node->keys[i])
            {
                SHIFTR(node->keys, i, node->key_count - 1);
                node->keys[i] = key;
                break;
            }
            else if (i == node->key_count - 1)
            {
                node->keys[node->key_count] = key;
            }
        }
    }

    node->key_count++;

#ifdef PRINT_PROCESS
    printf("key: %d -> Add key to the %s <%d %d %d>\n", key, is_leaf(node) ? "leaf" : "node", node->keys[0], node->keys[1], node->keys[2]);
#endif
}

void remove_key(Node_t *node, int key_id)
{
    SHIFTL(node->keys, key_id + 1, node->key_count);
    node->key_count--;
}

int is_leaf(Node_t *node)
{
    if (node == NULL)
    {
        return 0;
    }
    return ((node->left == NULL) && (node->right == NULL));
}

void split(Node_t *node, int key)
{
#ifdef PRINT_PROCESS
    printf("%d -> key: Spliting node <%d %d %d>\n", key, node->keys[0], node->keys[1], node->keys[2]);
#endif

    add_key(node, key);
    node->left = NewNode(node->keys[0], node);
    node->right = NewNode(node->keys[3], node);

    remove_key(node, 0);
    remove_key(node, 3);
    node->h += 1;
}

void treeprint(Node_t *root, int level, char c)
{
    if (root == NULL)
        return;

    for (int i = 0; i < level; i++)
    {
        printf(i == level - 1 ? "\t->" : "\t");
    }

    printf("%c:<", c);
    for (int i = 0; i < root->key_count; i++)
    {
        printf(" %d ", root->keys[i]);
    }
    printf(">, h:%d, parent <", height_of(root));

    if (root->parent)
    {
        for (int i = 0; i < root->parent->key_count; i++)
        {
            printf(" %d ", root->parent->keys[i]);
        }
    }
    printf(">\n");

    treeprint(root->left, level + 1, 'l');
    treeprint(root->right, level + 1, 'r');
}
