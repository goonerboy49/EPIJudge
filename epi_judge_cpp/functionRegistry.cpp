#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;
struct Function {
    Function(string _name, vector<string> _args, bool _isVariadic) {
        name = _name;
        args = _args;
        isVariadic = _isVariadic;
    }
    string name;
    vector<string> args;
    bool isVariadic;
};

struct TrieNode;
using TrieNodePtr = shared_ptr<TrieNode>;

struct TrieNode {
    TrieNode(string _arg) {
        arg = _arg;
    }
    string arg;
    std::unordered_map<string, TrieNodePtr> children;
    vector<Function> regFuncs;
    vector<Function> variadicFuncs;
};

void Register(const TrieNodePtr& root, const vector<Function>& functions) {
    for (const auto& func : functions) {
        TrieNodePtr curr = root;
        for(const auto& arg : func.args) {
            auto iter = curr->children.find(arg);
            if (iter != curr->children.end()) {
               //cout << "Found existing node for " << arg << endl;
               curr = iter->second;
            } else {
                //cout << "Adding node for " << arg << " curr arg is " << curr->arg << endl;
                TrieNodePtr child = std::make_shared<TrieNode>(arg);
                curr->children[arg] = child;
                curr = child;
            }
        }

        if (func.isVariadic) {
            //cout << "Pushing variadic func " << func.name << endl;
            curr->variadicFuncs.push_back(func);
        } else {
            //cout << "Pushing regular func " << func.name << endl;
            curr->regFuncs.push_back(func);
        }
    }
}

vector<Function> FindVariadicMatches(const TrieNodePtr& root, const vector<string>& args, int nextIdx)
{
    while(nextIdx < args.size()) {
        const auto& arg = args[nextIdx];
        if (arg != root->arg) {
            return vector<Function>{};
        }
        ++nextIdx;
    }

    return root->variadicFuncs;
}

vector<Function> FindMatches(const TrieNodePtr& root, const vector<string>& args, int nextIdx) {
    vector<Function> retVal;
    vector<Function> regMatches;
    vector<Function> variadicMatches;

    //cout << "Finding matches for idx " << nextIdx << " curr arg " << root->arg << endl;
    if (nextIdx == args.size()) {
        //cout << "Reached end of args adding reg " << root->regFuncs.size() << " and variadic " << root->variadicFuncs.size() << endl;
        retVal.insert(retVal.begin(), root->regFuncs.begin(), root->regFuncs.end());
        retVal.insert(retVal.begin(), root->variadicFuncs.begin(), root->variadicFuncs.end());
        return retVal;
    }

    const auto& arg = args[nextIdx];
    auto iter = root->children.find(arg);

    if (iter == root->children.end()) {
        regMatches = vector<Function>{};
    } else {
        regMatches = FindMatches(iter->second, args, nextIdx+1);
    }

    if (!root->variadicFuncs.empty()) {
        variadicMatches = FindVariadicMatches(root, args, nextIdx+1);
    }

    retVal.insert(retVal.begin(), regMatches.begin(), regMatches.end());
    retVal.insert(retVal.begin(), variadicMatches.begin(), variadicMatches.end());

    return retVal;
}

void TestCase(const TrieNodePtr& root, vector<string>& args) {
    auto f = [](vector<string> args) -> string {
        string ret;
        for(auto& s : args) {
            ret.append(s);
            ret.append(", ");
        }

        return ret;
    };

    auto matches = FindMatches(root, args, 0);
    cout << "Input " << f(args) << " matches: " <<  matches.size() << endl;
    for(auto& match : matches) {
        cout << match.name << " args: " << f(match.args) << " is variadic " << match.isVariadic << endl;
    }
}

int main() {    
    vector<Function> funcs = { Function("FuncA", vector<string> {"Integer", "Integer"}, false),
                               Function("FuncB", vector<string> {"Integer", "Integer"}, false),
                               Function("FuncC", vector<string> {"Boolean", "Integer"}, false),
                               Function("FuncD", vector<string> {"Boolean", "Integer"}, true),
                               Function("FuncE", vector<string> {"Boolean", "Boolean"}, false),
                               Function("FuncF", vector<string> {"Boolean", "Boolean"}, true),
                               Function("FuncG", vector<string> {"Integer"}, true), };

    TrieNodePtr root = std::make_shared<TrieNode>("");
    Register(root, funcs);

    {
        vector<string> args {"Integer"};
        TestCase(root, args);
    }

    {
        vector<string> args {};
        TestCase(root, args);
    }

    {
        vector<string> args {"Boolean", "Boolean"};
        TestCase(root, args);
    }

    {
        vector<string> args {"Boolean", "Boolean", "Boolean"};
        TestCase(root, args);
    }    

    {
        vector<string> args {"String", "Boolean", "Boolean"};
        TestCase(root, args);
    }

    {
        vector<string> args {"Boolean", "Integer", "Integer"};
        TestCase(root, args);
    }    

    return 0;
}