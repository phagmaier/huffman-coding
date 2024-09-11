#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <utility>

class Node{
public:
  Node(char l, char r);
  Node(Node *l, Node *r);
  Node(char l, Node *r);
  Node(Node *l, char r);
  bool left_is_node;
  bool right_is_node;
  std::pair<char, Node*> left;
  std::pair<char, Node*> right;
};

Node::Node(char l, char r){
  left_is_node = false;
  right_is_node = false;

  left= {l, nullptr};
  right = {r, nullptr};
}

Node::Node(Node *l, Node *r){
  left_is_node = true;
  right_is_node = true;

  left= {'\0', l};
  right = {'\0', r};
}

Node::Node(char l, Node *r){
  left_is_node = false;
  right_is_node = true;

  left= {l, nullptr};
  right = {'\0', r};
}


Node::Node(Node *l, char r) {
    left_is_node = true;   // Set to true, since left is a Node
    right_is_node = false; // Set to false, since right is a char
    left = {'\0', l};
    right = {r, nullptr};
}

class Either{
public:
  Either(char str) : str{str}, node{nullptr},is_node{false}{}
  Either(Node *node) : str{'\0'},node{node},is_node{true}{}
  char str;
  Node* node;
  bool is_node;
};

std::vector<std::pair<char,int>> merge(std::vector<std::pair<char,int>> left, std::vector<std::pair<char,int>> right){
    int l =  0;
    int r = 0;
    std::vector<std::pair<char,int>> sorted;
    while (l < left.size() && r < right.size()){
        if (left[l].second <= right[r].second){
            sorted.push_back(left[l]);
            l++;
        }
        else{
            sorted.push_back(right[r]);
            r++;
        }
    }
    // Add any remaining elements from both subarrays
    while (l < left.size()){
        sorted.push_back(left[l]);
        l++;
    }
    while (r < right.size()){
        sorted.push_back(right[r]);
        r++;
    }
    return sorted;
}

std::vector<std::pair<char,int>> merge_sort(std::vector<std::pair<char,int>> &vec){
    // Base case for empty and single element vectors
    if (vec.size() <= 1){
        return vec;
    }
    
    // Splitting the vector into two halves
    int mid = vec.size()/2;
    std::vector<std::pair<char,int>> left;
    std::vector<std::pair<char,int>> right;

    // Fill left and right subarrays
    for (int i=0; i<mid; ++i){
        left.push_back(vec[i]);
    }
    for (int i=mid; i<vec.size(); ++i){
        right.push_back(vec[i]);
    }
    
    // Merge recursively
    return merge(merge_sort(left), merge_sort(right));
}

std::vector<std::pair<char,int>> get_freq(std::string &str){
  std::unordered_map<char,int> map;
  std::vector<std::pair<char,int>> sorted;
  for (char c : str){
   map[c] +=1; 
  }
  for (auto &i : map){
    sorted.push_back({i.first,i.second});
  }
  //in reality gonna have to sort it
  return sorted;
}

void add_item(std::vector<std::pair<Either,int>> &vec, Node *node, int val){
  vec.erase(vec.begin(), vec.begin() + 2);
  if (vec.empty() || vec.back().second < val) { // Use vec.back() instead of vec[vec.size()-1]
    vec.push_back({Either(node), val});
    return;
  }
  for (int i = 0; i < vec.size(); ++i) {
    if (vec[i].second > val) {
      vec.insert(vec.begin() + i, {Either(node), val});
      return;
    }
  }
}

Node *make_tree(std::vector<std::pair<char,int>> &vec){
  std::vector<std::pair<Either,int>> new_vec;
  Node *tmp;
  int val;
  for (std::pair<char,int> &i : vec){
    new_vec.push_back({Either(i.first),i.second});
  }
  while (new_vec.size() >1){

    val = new_vec[0].second + new_vec[1].second;
    if(new_vec[0].first.is_node==true && new_vec[1].first.is_node==true){
      tmp = new Node(new_vec[0].first.node,new_vec[1].first.node);
    }
    else if(new_vec[0].first.is_node==false && new_vec[1].first.is_node==false){
      tmp = new Node(new_vec[0].first.str,new_vec[1].first.str);
  }
    else if(new_vec[0].first.is_node==true&& new_vec[1].first.is_node==false){
      tmp = new Node(new_vec[0].first.node,new_vec[1].first.str);
  }
    else{
      tmp = new Node(new_vec[0].first.str,new_vec[1].first.node);
  }
    add_item(new_vec,tmp,val);
  }
  return new_vec[0].first.node;
}
/*
  Main function implementing huffman coding
def huffman_code_tree(node, left=True, binString=''):
    if type(node) is str:
        return {node: binString}
    (l, r) = node.children()
    d = dict()
    d.update(huffman_code_tree(l, True, binString + '0'))
    d.update(huffman_code_tree(r, False, binString + '1'))
    return d
*/

void update_map(std::unordered_map<char,std::string> &dic, std::unordered_map<char,std::string> &dic2){
  for (auto &i : dic2){
    dic[i.first] = i.second;
  }
}

void get_huffman_codes(Node *node, std::unordered_map<char,std::string> &dic, std::string binstring) {
    if (node->left_is_node) {
        get_huffman_codes(node->left.second, dic, binstring + '0');
    } else {
        dic[node->left.first] = binstring + '0';  // Append '0' for left child
    }

    if (node->right_is_node) {
        get_huffman_codes(node->right.second, dic, binstring + '1');
    } else {
        dic[node->right.first] = binstring + '1';  // Append '1' for right child
    }
}

int main(){
  std::string str = "Hello world my name is parker this is a string about something random fejni3fhr";
  std::vector<std::pair<char,int>> freq = get_freq(str);
  std::vector<std::pair<char,int>>merged = merge_sort(freq);
  for (auto &i : merged){
    std::cout << i.first << ": " << i.second << "\n";
  }
  std::cout << "\n";
  Node *node = make_tree(merged);
  std::unordered_map<char,std::string> dic;
  get_huffman_codes(node,dic,"");
  std::cout << "------------------------\n";
  for (auto &i : dic){
    std::cout << i.first << "\t|" << "\t"<< i.second <<"\n";

  }
  
  std::cout << "\n\n\n";
  
  int count = 0;
  for (auto &i : dic){
    count += size(i.second);
  }
  std::cout << count << "\n";

  return 0;

}
