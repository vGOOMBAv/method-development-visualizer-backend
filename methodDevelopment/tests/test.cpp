//Author: Anczykowski Igor

#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/included/unit_test.hpp>

using namespace boost::unit_test;
using namespace boost::unit_test_framework;

#include "../src/Node.hpp"
#include "../src/Tree.hpp"
#include <iostream>

BOOST_AUTO_TEST_SUITE( Node_suite )
    BOOST_AUTO_TEST_CASE( Node_creation_empty ) {
        Node node("SHADE");
        BOOST_REQUIRE(node.getName() == "SHADE");
        BOOST_REQUIRE(node.getDescription().empty());
        BOOST_REQUIRE(node.getAliases().empty());
    }
    BOOST_AUTO_TEST_CASE( Node_creation_desc ) {
        Node node("SHADE", "Description...");
        BOOST_REQUIRE(node.getName() == "SHADE");
        BOOST_REQUIRE(node.getDescription() == "Description...");
        BOOST_REQUIRE(node.getAliases().empty());
    }
    BOOST_AUTO_TEST_CASE( Node_creation_desc_and_aliases ) {
        std::vector<std::string> aliases;
        aliases.emplace_back("shade");
        aliases.emplace_back("Shade");
        Node node("SHADE", "Description...", aliases);
        BOOST_REQUIRE(node.getName() == "SHADE");
        BOOST_REQUIRE(node.getDescription() == "Description...");
        BOOST_REQUIRE(node.getAliases().at(0) == "shade");
        BOOST_REQUIRE(node.getAliases().at(1) == "Shade");
    }
    BOOST_AUTO_TEST_CASE( Node_setter ) {
        std::vector<std::string> aliases;
        aliases.emplace_back("shade");
        aliases.emplace_back("Shade");
        Node node("SHADE", "Description...", aliases);
        BOOST_REQUIRE(node.getName() == "SHADE");
        BOOST_REQUIRE(node.getDescription() == "Description...");
        BOOST_REQUIRE(node.getAliases().at(0) == "shade");
        BOOST_REQUIRE(node.getAliases().at(1) == "Shade");
        aliases.emplace_back("shaDE");
        node.setAliases(aliases);
        node.setDescription("Description2");
        node.setName("SHADE_NAME");
        BOOST_REQUIRE(node.getName() == "SHADE_NAME");
        BOOST_REQUIRE(node.getDescription() == "Description2");
        BOOST_REQUIRE(node.getAliases().at(0) == "shade");
        BOOST_REQUIRE(node.getAliases().at(1) == "Shade");
        BOOST_REQUIRE(node.getAliases().at(2) == "shaDE");
    }
    BOOST_AUTO_TEST_CASE( Node_add_and_remove_alias ) {
        std::vector<std::string> aliases;
        Node node("SHADE", "Description...", aliases);
        BOOST_REQUIRE(node.getAliases().empty());
        node.addAlias("Shade");
        BOOST_REQUIRE(node.getAliases().at(0) == "Shade");
        node.addAlias("shade");
        BOOST_REQUIRE(node.getAliases().at(1) == "shade");
        node.addAlias("shaDE");
        BOOST_REQUIRE(node.getAliases().at(2) == "shaDE");
        node.removeAlias("shade");
        BOOST_REQUIRE(node.getAliases().at(0) == "Shade");
        BOOST_REQUIRE(node.getAliases().at(1) == "shaDE");
    }
    BOOST_AUTO_TEST_CASE( Node_add_and_remove_children ) {
        auto node = std::make_shared<Node>("node");
        auto node_1 = std::make_shared<Node>("node_1");
        auto node_2 = std::make_shared<Node>("node_2");
        auto node_3 = std::make_shared<Node>("node_3");

        BOOST_REQUIRE( node->getChildren().empty() );
        BOOST_REQUIRE( node->getParent() == nullptr );
        node->addChild(node_1);
        BOOST_REQUIRE( node->getChildren().at(0)->getName() == "node_1" );
        node->addChild(node_2);
        BOOST_REQUIRE( node->getChildren().at(0)->getName() == "node_1" );
        BOOST_REQUIRE( node->getChildren().at(1)->getName() == "node_2" );
        node->addChild(node_3);
        BOOST_REQUIRE( node->getChildren().at(0)->getName() == "node_1" );
        BOOST_REQUIRE( node->getChildren().at(1)->getName() == "node_2" );
        BOOST_REQUIRE( node->getChildren().at(2)->getName() == "node_3" );
        node->removeChild("node_2");
        BOOST_REQUIRE( node->getChildren().at(0)->getName() == "node_1" );
        BOOST_REQUIRE( node->getChildren().at(1)->getName() == "node_3" );
    }
    BOOST_AUTO_TEST_CASE( Node_parent ) {
        auto node = std::make_shared<Node>("node");
        auto node_1 = std::make_shared<Node>("node_1");
        auto node_2 = std::make_shared<Node>("node_2");
        auto node_3 = std::make_shared<Node>("node_3");

        node_2->addChild(node_3);

        BOOST_REQUIRE( node->getChildren().empty() );
        BOOST_REQUIRE( node->getParent() == nullptr );
        node->addChild(node_1);
        BOOST_REQUIRE( node->getChildren().at(0)->getName() == "node_1" );
        BOOST_REQUIRE( node->getChildren().at(0)->getParent()->getName() == "node" );
        node->addChild(node_2);
        BOOST_REQUIRE( node->getChildren().at(0)->getName() == "node_1" );
        BOOST_REQUIRE( node->getChildren().at(1)->getName() == "node_2" );
        BOOST_REQUIRE( node->getChildren().at(0)->getParent()->getName() == "node" );
        BOOST_REQUIRE( node->getChildren().at(1)->getParent()->getName() == "node" );

        BOOST_REQUIRE( node->getChildren().at(1)->getChildren().at(0)->getParent()->getName() == "node_2" );
    }
    BOOST_AUTO_TEST_CASE( Node_ownership_transfer ) {
        auto node = std::make_shared<Node>("node");
        {
            auto node_1 = std::make_shared<Node>("node_1");
            BOOST_REQUIRE(node->getChildren().empty());
            node->addChild(node_1);
            BOOST_REQUIRE(node->getChildren().at(0)->getName() == "node_1");
        }
        BOOST_REQUIRE(node->getChildren().at(0)->getName() == "node_1");
    }

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE( Tree_suite )
    BOOST_AUTO_TEST_CASE( Tree_creation ) {
        auto node = std::make_shared<Node>("node");
        Tree tree(node);
        BOOST_REQUIRE(tree.getRootNode()->getName() == "node");
    }
    BOOST_AUTO_TEST_CASE( Tree_set_root ) {
        auto node = std::make_shared<Node>("node");
        auto node_1 = std::make_shared<Node>("node_1");
        Tree tree(node);
        BOOST_REQUIRE(tree.getRootNode()->getName() == "node");
        tree.setRootNode(node_1);
        BOOST_REQUIRE(tree.getRootNode()->getName() == "node_1");
    }
    BOOST_AUTO_TEST_CASE( Tree_find_node ) {
        auto node = std::make_shared<Node>("node");
        auto node_1 = std::make_shared<Node>("node_1");
        auto node_1_1 = std::make_shared<Node>("node_1_1");
        auto node_1_2 = std::make_shared<Node>("node_1_2");
        auto node_2 = std::make_shared<Node>("node_2");
        auto node_3 = std::make_shared<Node>("node_3");
        node_1->addChild(node_1_1);
        node_1->addChild(node_1_2);
        node->addChild(node_1);
        node->addChild(node_2);
        node->addChild(node_3);
        Tree tree(node);
        BOOST_REQUIRE(tree.findNode("node")->getName() == "node");
        BOOST_REQUIRE(tree.findNode("node that do not exists") == nullptr);
        BOOST_REQUIRE(tree.findNode("node_1")->getName() == "node_1");
        BOOST_REQUIRE(tree.findNode("node_1_1")->getName() == "node_1_1");
    }
    BOOST_AUTO_TEST_CASE( Tree_remove_node ) {
        auto node = std::make_shared<Node>("node");
        auto node_1 = std::make_shared<Node>("node_1");
        auto node_1_1 = std::make_shared<Node>("node_1_1");
        auto node_1_2 = std::make_shared<Node>("node_1_2");
        auto node_2 = std::make_shared<Node>("node_2");
        auto node_3 = std::make_shared<Node>("node_3");
        node_1->addChild(node_1_1);
        node_1->addChild(node_1_2);
        node->addChild(node_1);
        node->addChild(node_2);
        node->addChild(node_3);
        Tree tree(node);
        BOOST_REQUIRE(tree.findNode("node")->getName() == "node");
        BOOST_REQUIRE(tree.findNode("node_1")->getName() == "node_1");
        BOOST_REQUIRE(tree.findNode("node_1_1")->getName() == "node_1_1");
        tree.removeNode("node_1_1");
        BOOST_REQUIRE(tree.findNode("node")->getName() == "node");
        BOOST_REQUIRE(tree.findNode("node_1")->getName() == "node_1");
        BOOST_REQUIRE(tree.findNode("node_1_1") == nullptr);
        tree.removeNode("node_1");
        BOOST_REQUIRE(tree.findNode("node")->getName() == "node");
        BOOST_REQUIRE(tree.findNode("node_1") == nullptr);
        BOOST_REQUIRE(tree.findNode("node_1_1") == nullptr);
        BOOST_REQUIRE(tree.findNode("node_1_2") == nullptr);
    }
BOOST_AUTO_TEST_SUITE_END()