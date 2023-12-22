#pragma once
#include <iostream>
#include <vector>
#include <set>

class Tree
{
    class Node
    {
        public:
            int id;
            Node* left;
            Node* right;
            int height;
            bool available;

            Node(int id)
            {
                this -> id = id;
                available = true;
                left = NULL;
                right = NULL;
            }

            void Check_availability(int id)
            {
                if (this -> id == id)
                {
                    available = false;
                }

                else
                {
                    if (left != NULL)
                    {
                        left -> Check_availability(id);
                    }

                    if (right != NULL)
                    {
                        right -> Check_availability(id);
                    }
                }
            }

            void Remove(int id, std::set<int> &ids)
            {
                if (left != NULL && left -> id == id)
                {
                    left -> Recursion_remove(ids);
                    ids.erase(left -> id);
                    delete left;
                    left = NULL;
                }

                else if (right != NULL && right -> id == id)
                {
                    right -> Recursion_remove(ids);
                    ids.erase(right -> id);
                    delete right;
                    right = NULL;
                }

                else
                {
                    if (left != NULL)
                    {
                        left -> Remove(id, ids);
                    }

                    if (right != NULL)
                    {
                        right -> Remove(id, ids);
                    }
                }
            }


            void Recursion_remove(std::set<int> &ids)
            {
                if (left != NULL)
                {
                    left -> Recursion_remove(ids);
                    ids.erase(left -> id);
                    delete left;
                    left = NULL;
                }

                if (right != NULL)
                {
                    right -> Recursion_remove(ids);
                    ids.erase(right -> id);
                    delete right;
                    right = NULL;
                }
            }

            void Add_node(int id, int parent_id, std::set<int> &ids)
            {
                if (this -> id == parent_id)
                {
                    if (left == NULL)
                    {
                        left = new Node(id);
                    }

                    else
                    {
                        right = new Node(id);
                    }

                    ids.insert(id);
                }

                else
                {
                    if (left != NULL)
                    {
                        left -> Add_node(id, parent_id, ids);
                    }

                    if (right != NULL)
                    {
                        right -> Add_node(id, parent_id, ids);
                    }
                }
            }

            int Minimal_height()
            {
                if (left == NULL || right == NULL)
                {
                    return 0;
                }

                int left_height = -1, right_height = -1;

                if (left != NULL && left -> available == true)
                {
                    left_height = left -> Minimal_height();
                }

                if (right != NULL && right -> available == true)
                {
                    right_height = right -> Minimal_height();
                }

                if (right_height == -1 && left_height == -1)
                {
                    available = false;
                    return -1;
                }

                else if (right_height == -1)
                {
                    return left_height + 1;
                }

                else if (left_height == -1)
                {
                    return right_height + 1;
                }

                else
                {
                    return std::min(left_height, right_height) + 1;
                }
            }

            int ID_minimal_height(int height, int current_height)
            {
                if (height < current_height)
                {
                    return -2;
                }

                else if (height > current_height)
                {
                    int current_id = -2;

                    if (left != NULL && left -> available == true)
                    {
                        current_id = left -> ID_minimal_height(height, (current_height + 1));
                    }

                    if (right != NULL && right -> available == true && current_id == -2)
                    {
                        current_id = right -> ID_minimal_height(height, (current_height + 1));
                    }

                    return current_id;
                }

                else
                {
                    if (left == NULL || right == NULL)
                    {
                        return id;
                    }

                    return -2;
                }
            }

            ~Node() {}
    };

    private:
        Node* root;

    public:
        std::set<int> ids;

        Tree()
        {
            root = new Node(-1);
        }

        bool Exist(int id)
        {
            if (ids.find(id) != ids.end())
            {
                return true;
            }

            return false;

            // Можно просто return ids.find(id) != ids.end()
        }

        void Availability_check(int id)
        {
            root -> Check_availability(id);
        }

        int FindID()
        {
            int min_height = root -> Minimal_height();
            return root -> ID_minimal_height(min_height, 0);
        }

        void Add(int id, int parent)
        {
            root -> Add_node(id, parent, ids);
        }

        void Remove_from_tree(int id)
        {
            root -> Remove(id, ids);
        }

        ~Tree()
        {
            root -> Recursion_remove(ids);
            delete root;
        }
};