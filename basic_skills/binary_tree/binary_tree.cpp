
#include "binary_tree.h"

#include <cstddef>
#include <iostream>
#include <queue>
#include <string>

#include "FileOps.h"
#include "SequenceST.h"

template <typename Key, typename Value>
class BST {
private:
    struct Node {
        Key key;
        Value value;
        Node* left;
        Node* right;

        Node(Key key, Value value) {
            this->key = key;
            this->value = value;
            this->left = this->right = nullptr;
        }

        Node(Node* node) {
            this->key = node->key;
            this->value = node->value;
            this->left = node->left;
            this->right = node->right;
        }
    };

    Node* root_;
    int count_;

public:
    BST() {
        root_ = nullptr;
        count_ = 0;
    };
    ~BST() {
        destory();
    };

    int size() {
        return count_;
    }

    bool is_empty() {
        return count_ == 0;
    }

    void insert(Key key, Value value) {
        root_ = insert(root_, key, value);
    }

    bool contain(Key key) {
        return contain(root_, key);
    }

    Value* search(Key key) {
        return search(root_, key);
    }

    void pre_order() {
        pre_order(root_);
    }
    void in_order() {
        in_order(root_);
    }
    void post_order() {
        post_order(root_);
    }
    void level_order() {
        std::queue<Node*> q;
        q.push(root_);

        while (!q.empty()) {
            Node* node = q.front();
            q.pop();
            std::cout << node->key << std::endl;

            if (node->left) {
                q.push(node->left);
            }
            if (node->right) {
                q.push(node->right);
            }
        }
    }

    void destory() {
        destory_node(root_);
    }

    Key min() {
        assert(count_ != 0);
        return min(root_)->key;
    }
    Key max() {
        assert(count_ != 0);
        return max(root_)->key;
    }
    void remove_min() {
        if (root_)
            remove_min(root_);
    }
    void remove_max() {
        if (root_)
            remove_max(root_);
    }

    void remove_node(Key key) {
        root_ = remove_node(root_, key);
    }

private:
    Node* insert(Node* node, Key key, Value value) {
        if (node == nullptr) {
            count_++;
            return new Node(key, value);
        }

        if (node->key == key) {
            node->value = value;
        } else if (key < node->key) {
            node->left = insert(node->left, key, value);
        } else {
            node->right = insert(node->right, key, value);
        }
        return node;
    }

    bool contain(Node* node, Key key) {
        if (node == nullptr) {
            return false;
        }

        if (node->key == key) {
            return true;
        } else if (node->key > key) {
            return contain(node->left, key);
        } else {
            return contain(node->right, key);
        }
    }

    Value* search(Node* node, Key key) {
        if (node == nullptr) {
            return nullptr;
        }

        if (node->key == key) {
            return &(node->value);
        } else if (node->key > key) {
            return search(node->left, key);
        } else {
            return search(node->right, key);
        }
    }

    void pre_order(Node* node) {
        if (node != nullptr) {
            std::cout << node->key << " " << node->value << std::endl;
            pre_order(node->left);
            pre_order(node->right);
        }
    }

    void in_order(Node* node) {
        if (node != nullptr) {
            pre_order(node->left);
            std::cout << node->key << " " << node->value << std::endl;
            pre_order(node->right);
        }
    }

    void post_order(Node* node) {
        if (node != nullptr) {
            pre_order(node->left);
            pre_order(node->right);
            std::cout << node->key << " " << node->value << std::endl;
        }
    }

    void destory_node(Node* node) {
        if (node != nullptr) {
            destory_node(node->left);
            destory_node(node->right);
            delete node;
        }
    }

    Node* min(Node* node) {
        if (node->left == nullptr) {
            return node;
        }
        return min(node->left);
    }

    Node* max(Node* node) {
        if (node->right == nullptr) {
            return node;
        }
        return max(node->right);
    }

    void remove_min(Node* node) {
        if (node->left == nullptr) {
            Node* right_node = node->right;
            delete node;
            count_--;
            return right_node;
        }

        node->left = remove_min(node->left);
        return node;
    }

    void remove_max(Node* node) {
        if (node->right == nullptr) {
            Node* left_node = node->left;
            delete node;
            count_--;
            return left_node;
        }

        node->right = remove_max(node->right);
        return node;
    }

    Node* remove_node(Node* node, Key key) {
        if (node == nullptr) {
            return nullptr;
        }

        if (node->key > key) {
            node->left = remove_node(node->left, key);
            return node;
        } else if (node->key < key) {
            node->right = remove_node(node->right, key);
            return node;
        } else {
            if (node->left == nullptr) {
                Node* right_node = node->right;
                delete node;
                count_--;
                return right_node;
            }

            if (node->right == nullptr) {
                Node* left_node = node->left;
                delete node;
                count_--;
                return left_node;
            }

            Node* new_node = new Node(min(node->right));
            count_++;
            new_node->right = remove_min(node->right);
            new_node->left = node->left;
            delete node;
            count_--;
            return new_node;
        }
    }
};

void shuffle(int arr[], int n) {
    srand(time(NULL));
    for (int i = n - 1; i >= 0; i--) {
        int x = rand() % (i + 1);
        swap(arr[i], arr[x]);
    }
}

int run_binary_tree() {
    std::string filename = "../binary_tree/bible.txt";
    std::vector<std::string> words;
    if (FileOps::readFile(filename, words)) {
        std::cout << "There are totally " << words.size() << " words in " << filename << std::endl;

        std::cout << std::endl;

        // test BST
        clock_t startTime = clock();
        BST<std::string, int> bst = BST<std::string, int>();
        for (std::vector<std::string>::iterator iter = words.begin(); iter != words.end(); iter++) {
            int* res = bst.search(*iter);
            if (res == NULL)
                bst.insert(*iter, 1);
            else
                (*res)++;
        }

        std::cout << "'god' : " << *bst.search("god") << std::endl;
        clock_t endTime = clock();
        std::cout << "BST , time: " << double(endTime - startTime) / CLOCKS_PER_SEC << " s." << std::endl;

        std::cout << std::endl;

        // test SST
        startTime = clock();
        SequenceST<std::string, int> sst = SequenceST<std::string, int>();
        for (std::vector<std::string>::iterator iter = words.begin(); iter != words.end(); iter++) {
            int* res = sst.search(*iter);
            if (res == NULL)
                sst.insert(*iter, 1);
            else
                (*res)++;
        }

        std::cout << "'god' : " << *sst.search("god") << std::endl;

        endTime = clock();
        std::cout << "SST , time: " << double(endTime - startTime) / CLOCKS_PER_SEC << " s." << std::endl;
    }

    return 0;
}