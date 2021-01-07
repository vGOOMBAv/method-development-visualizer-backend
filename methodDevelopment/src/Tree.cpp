//Author: Anczykowski Igor

#include "Tree.hpp"
#include <queue>

Tree::Tree(const std::shared_ptr<Node> &root_node) : root_node_(root_node) {} //NOLINT
std::shared_ptr<Node> Tree::getRootNode() const {
    return this->root_node_;
}
void Tree::setRootNode(const std::shared_ptr<Node> &root_node) {
    this->root_node_ = root_node;
}
std::shared_ptr<Node> Tree::findNode(const std::string &node_name) const {
    std::queue<std::shared_ptr<Node>> q;
    q.push(this->root_node_);
    // BFS algorithm
    while (!q.empty()) {
        // See if current node is same as x
        std::shared_ptr<Node> node = q.front();
        if (node->getName() == node_name)
            return node;
        q.pop();
        for(const auto& node_ : node->getChildren()) {
            q.push(node_);
        }
    }
    return nullptr;
}
void Tree::removeNode(const std::string &node_name) const {
    auto parent_of_searched = this->findNode(node_name)->getParent();
    if(parent_of_searched != nullptr){
        parent_of_searched->removeChild(node_name);
    }
}
std::vector<std::shared_ptr<Node>> Tree::getAllNodes() {
    std::vector<std::shared_ptr<Node>> all_nodes;
    std::queue<std::shared_ptr<Node>> q;
    q.push(this->root_node_);
    // BFS algorithm
    while (!q.empty()) {
        std::shared_ptr<Node> node = q.front();
        q.pop();
        all_nodes.push_back(node);
        for(const auto& node_ : node->getChildren()) {
            q.push(node_);
        }
    }
    return all_nodes;
}
void Tree::addNode(const std::shared_ptr<Node> &node, const std::string &parent_name) {
    if(parent_name.empty()){
        this->setRootNode(node);
        return;
    }
    auto parent = this->findNode(parent_name);
    parent->addChild(node);
}
