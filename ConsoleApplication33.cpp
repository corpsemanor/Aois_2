#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stack>

using namespace std;


vector<string> getSubformulas(string formula)
{
    vector<string> subformulas;

    stack<int> openingBrackets;
    for (int i = 0; i < formula.size(); i++) {
        if (formula[i] == '(') {
            openingBrackets.push(i);
        }
    }

    while (!openingBrackets.empty()) {
        int startIndex = openingBrackets.top();
        openingBrackets.pop();

        int openingCount = 1;
        int closingCount = 0;
        int currentIndex = startIndex + 1;

        while (openingCount != closingCount && currentIndex < formula.size()) {
            if (formula[currentIndex] == '(') {
                openingCount++;
            }
            else if (formula[currentIndex] == ')') {
                closingCount++;
            }
            currentIndex++;
        }

        subformulas.push_back(formula.substr(startIndex + 1, currentIndex - startIndex - 2));
    }
    return subformulas;
}

vector<string> getVariables(string formula)
{
    vector<string> vars;
    string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (char symbol : formula)
    {
        if (alphabet.find(symbol) != string::npos)
        {
            string buffer;
            buffer.push_back(symbol);
            vars.push_back(buffer);
        }
    }

    sort(vars.begin(), vars.end());
    vars.erase(std::unique(vars.begin(), vars.end()), vars.end());

    return vars;
}

vector<string> normalizeSubformulas(string formula)
{
    string allowed_symbols = "\\/!()~->";
    vector<string> subformulas = getSubformulas(formula);
    for (int i = 0; i < subformulas.size(); i++)
    {
        if (subformulas[i].size() != 1)
        {
            for (char symbol : subformulas[i])
            {
                if (allowed_symbols.find(symbol) == string::npos &&
                    subformulas[i][0] != '(')
                {
                    reverse(subformulas[i].begin(), subformulas[i].end());
                    subformulas[i].push_back('(');
                    reverse(subformulas[i].begin(), subformulas[i].end());
                    subformulas[i].push_back(')');
                }
            }
        }
    }

    sort(subformulas.begin(), subformulas.end());
    subformulas.erase(std::unique(subformulas.begin(), subformulas.end()), subformulas.end());

    return subformulas;
}

string removeBrackets(string expression)
{
    int right_counter = 0;
    int left_counter = 0;
    string no_brackets_expr;
    for (int i = 0; i < expression.size(); i++)
    {
        if (expression[i] == '(')right_counter++;
        if (expression[i] == ')')left_counter++;
    }
    for (int i = 0; i < left_counter; i++)
    {
        expression.erase(expression.find(')'), 1);
    }
    for (int i = 0; i < right_counter; i++)
    {
        expression.erase(expression.find('('), 1);
    }
    return no_brackets_expr = expression;
}

bool compareStringLength(const string& a, const string& b) {
    return a.length() < b.length();
}

int getOperator(string formula, vector<string> all_subformulas)
{
    string found_operator = formula;
    int cur_operator = 0;
    int j = 0;

    if (normalizeSubformulas(formula).size() > 1)
    {

        if (formula.size() == all_subformulas[all_subformulas.size() - 1].size())
        {
            sort(all_subformulas.begin(), all_subformulas.end(), compareStringLength);
            all_subformulas.resize(all_subformulas.size() - 1);
        }

        reverse(all_subformulas.begin(), all_subformulas.end());
        for (int i = 0; i < all_subformulas.size(); i++)
        {
            if (all_subformulas[i] == formula)
            {
                reverse(all_subformulas.begin(), all_subformulas.end());
                for (int j = 0; j < i; j++)
                {
                    all_subformulas.resize(all_subformulas.size() - 1);
                }
                all_subformulas.resize(all_subformulas.size() - 1);

                reverse(all_subformulas.begin(), all_subformulas.end());
                
            }
        }

        for (int i = 0; i < all_subformulas.size(); i++)
        {
            size_t index = formula.find(all_subformulas[i]);
            while (index != string::npos)
            {
                formula.replace(index, all_subformulas[i].size(), "");
                index = formula.find(all_subformulas[i], index + 1);
            }
        }
        found_operator = formula;
        found_operator = removeBrackets(found_operator);

    }

    while (j != found_operator.size())
    {
        if (found_operator[j] == '-' && found_operator[j + 1] == '>')
        {
            cur_operator = 0;
        }
        if (found_operator[j] == '\\' && found_operator[j + 1] == '/')
        {
            cur_operator = 1;
        }
        if (found_operator[j] == '!')
        {
            cur_operator = 2;
        }
        if (found_operator[j] == '~')
        {
            cur_operator = 3;
        }
        if (found_operator[j] == '/' && found_operator[j + 1] == '\\')
        {
            cur_operator = 4;
        }
        j++;
    }   
    
    return cur_operator;
}

vector<vector<int>> createTruthTabe(string formula)
{
    vector<string> subformulas = normalizeSubformulas(formula);
    vector<string> vars = getVariables(formula);
    vector<bool> vars_bool;
    int table_size = pow(2, vars.size());
    vector<vector<int>> table(table_size, vector<int>(vars.size()));
    sort(subformulas.begin(), subformulas.end(), compareStringLength);
    cout << "\n\n";

    for (int i = 0; i < subformulas.size(); i++)
    {
        cout << i+1 <<")" << subformulas[i] << "\t";
    }


    for (int i = 0; i < vars.size(); i++)
    {
        vars_bool.push_back(0);
    }

    for (int i = 0; i < table_size; i++)
    {
        int value = i;
        for (int j = vars.size() - 1; j >= 0; j--)
        {
            table[i][j] = value % 2;
            value /= 2;
        }
    }

    cout << endl;
    for (int i = 0; i < vars.size(); i++)
    {
        cout << vars[i] << "\t";
    }
    for (int i = 0; i < subformulas.size(); i++)
    {
        cout << "SF" << i + 1 << "\t";
    }
    cout << "\n\n";

    for (int i = 0; i < table_size; i++)
    {
        for (int j = 0; j < vars.size(); j++)
        {
            cout << table[i][j] << "\t";
        }
        for (int k = 0; k < subformulas.size(); k++)
        {


            int subformula_operator;
            if (getOperator(subformulas[k], subformulas) == 0)
            {

                vector<string> sub_vars = getVariables(subformulas[k]);
                vector<int> indexes(2, 0);

                if (normalizeSubformulas(subformulas[k]).size() > 1)
                {
                    indexes.clear();
                    for (int i = 0; i < subformulas.size(); i++) {
                        if (subformulas[k].find(subformulas[i]) != std::string::npos) {
                            indexes.push_back(i);
                        }
                    }
                    cout << (!(bool(table[i][indexes[0] + vars.size() - 1])) || bool(table[i][indexes[1] + vars.size() - 1])) << "\t";
                    table[i].push_back(int((!(bool(table[i][indexes[0] + vars.size() - 1])) || bool(table[i][indexes[1] + vars.size() - 1]))));
                }
                else
                {
                    for (int n = 0; n < vars.size(); n++)
                    {
                        for (int m = 0; m < sub_vars.size(); m++)
                        {
                            if (sub_vars[m] == vars[n])indexes[m] = m;
                        }
                    }
                    cout << (!(bool(table[i][indexes[0]])) || bool(table[i][indexes[1]])) << "\t";
                    table[i].push_back(int((!(bool(table[i][indexes[0]])) || bool(table[i][indexes[1]]))));
                }




            }
            if (getOperator(subformulas[k], subformulas) == 1)
            {
                vector<string> sub_vars = getVariables(subformulas[k]);
                vector<int> indexes(2, 0);

                if (normalizeSubformulas(subformulas[k]).size() > 1)
                {
                    indexes.clear();
                    for (int i = 0; i < subformulas.size(); i++) {
                        if (subformulas[k].find(subformulas[i]) != std::string::npos) {
                            indexes.push_back(i);
                        }
                    }
                    cout << (bool(table[i][indexes[0] + vars.size() - 1]) || bool(table[i][indexes[1] + vars.size() - 1])) << "\t";
                    table[i].push_back(int((bool(table[i][indexes[0] + vars.size() - 1]) || bool(table[i][indexes[1] + vars.size() - 1]))));
                }
                else
                {
                    for (int n = 0; n < vars.size(); n++)
                    {
                        for (int m = 0; m < sub_vars.size(); m++)
                        {
                            if (sub_vars[m] == vars[n])indexes[m] = m;
                        }
                    }
                    cout << (bool(table[i][indexes[0]]) || bool(table[i][indexes[1]])) << "\t";
                    table[i].push_back(int((bool(table[i][indexes[0]]) || bool(table[i][indexes[1]]))));
                }


            }


            if (getOperator(subformulas[k], subformulas) == 2)
            {
                vector<string> sub_vars = getVariables(subformulas[k]);
                vector<int> indexes(2, 0);
                vector<string> reversed_subs = subformulas;
                reverse(reversed_subs.begin(), reversed_subs.end());
                if (normalizeSubformulas(subformulas[k]).size() > 1)
                {
                    indexes.clear();

                    for (int i = 0; i < reversed_subs.size(); i++)
                    {
                        if (reversed_subs[i] == subformulas[k])
                        {
                            reverse(reversed_subs.begin(), reversed_subs.end());
                            for (int j = 0; j < i; j++)
                            {
                                reversed_subs.resize(reversed_subs.size() - 1);
                            }
                            reversed_subs.resize(reversed_subs.size() - 1);

                            reverse(reversed_subs.begin(), reversed_subs.end());

                        }
                    }

                    for (int i = 0; i < reversed_subs.size(); i++)
                    {
                        if (subformulas[k].find(reversed_subs[i]) != std::string::npos)
                        {
                            indexes.push_back(i);
                            break;
                        }
                    }

                    for (int i = 0; i < subformulas.size(); i++)
                    {
                        for (int j = 0; j < reversed_subs[indexes[0]].size(); j++)
                        {
                            if (subformulas[i] == reversed_subs[indexes[0]])
                            {
                                indexes.clear();
                                indexes.push_back(i);
                            }
                        }
                    }
                    cout << (!(bool(table[i][indexes[0] + vars.size() - 1]))) << "\t";
                    table[i].push_back(int(!(bool(table[i][indexes[0] + vars.size() - 1]))));
                }
                else
                {
                    for (int n = 0; n < vars.size(); n++)
                    {
                        for (int m = 0; m < sub_vars.size(); m++)
                        {
                            if (sub_vars[m] == vars[n])indexes[m] = m;
                        }
                    }
                    cout << (!(bool(table[i][indexes[0]]))) << "\t";
                    table[i].push_back(int(!(bool(table[i][indexes[0]]))));
                }


            }

            if (getOperator(subformulas[k], subformulas) == 3)
            {
                vector<string> sub_vars = getVariables(subformulas[k]);
                vector<int> indexes(2, 0);


                if (normalizeSubformulas(subformulas[k]).size() > 1)
                {
                    indexes.clear();
                    for (int i = 0; i < subformulas.size(); i++) {
                        if (subformulas[k].find(subformulas[i]) != std::string::npos) {
                            indexes.push_back(i);
                        }
                    }
                }
                else
                {
                    for (int n = 0; n < vars.size(); n++)
                    {
                        for (int m = 0; m < sub_vars.size(); m++)
                        {
                            if (sub_vars[m] == vars[n])indexes[m] = m;
                        }
                    }
                    if (bool(table[i][indexes[0]]) == bool(table[i][indexes[1]]))
                    {
                        cout << "1" << "\t";
                        table[i].push_back(1);
                    }
                    else
                    {
                        cout << "0" << "\t";
                        table[i].push_back(0);
                    }
                }
            }
            if (getOperator(subformulas[k], subformulas) == 4)
            {
                vector<string> sub_vars = getVariables(subformulas[k]);
                vector<int> indexes(2, 0);

                if (normalizeSubformulas(subformulas[k]).size() > 1)
                {
                    indexes.clear();
                    for (int i = 0; i < subformulas.size(); i++) {
                        if (subformulas[k].find(subformulas[i]) != std::string::npos) {
                            indexes.push_back(i);
                        }
                    }
                    cout << (bool(table[i][indexes[0] + vars.size() - 1]) && bool(table[i][indexes[1] + vars.size() - 1])) << "\t";
                    table[i].push_back(int((bool(table[i][indexes[0] + vars.size() - 1]) && bool(table[i][indexes[1] + vars.size() - 1]))));
                }
                else
                {
                    for (int n = 0; n < vars.size(); n++)
                    {
                        for (int m = 0; m < sub_vars.size(); m++)
                        {
                            if (sub_vars[m] == vars[n])indexes[m] = m;
                        }
                    }
                    cout << (bool(table[i][indexes[0]]) && bool(table[i][indexes[1]])) << "\t";
                    table[i].push_back(int((bool(table[i][indexes[0]]) && bool(table[i][indexes[1]]))));
                }


            }
        }
        cout << endl;
    }
    return table;
}

vector<int> decimalToBinary(int decimal)
{
    int checkSign = decimal;
    vector<int> output;
    while (fabs(decimal))
    {
        output.push_back(abs(decimal) % 2);
        decimal = fabs(decimal) / 2;
    }
    while (output.size() < 16)output.push_back(0);
    reverse(output.begin(), output.end());
    if (checkSign < 0)output[0] = 1;
    return output;
}

int binaryToDecimal(vector<int> binary)
{
    int output = 0;
    reverse(binary.begin(), binary.end());
    for (int i = binary.size() - 2; i > -1; i--)
        output += pow(2, i) * binary[i];
    return output;
}

string toSDNF(vector<vector<int>> table, vector<string> vars)
{
    string sdnf;
    vector<vector<int>> cons;
    vector<int> buffer;
    int table_size = pow(2, vars.size());
    for (int i = 0; i < table_size; i++)
    {
        if ((table[i][table[i].size() - 1]) == 1)
        {
            for (int j = 0; j < vars.size(); j++)
            {
                buffer.push_back(table[i][j]);
            }
            cons.push_back(buffer);
            buffer.clear();
        }
    }
    for (int i = 0; i < cons.size(); i++)
    {
        sdnf.push_back('(');
        for (int j = 0; j < cons[i].size(); j++)
        {
            sdnf.push_back('(');
            if (cons[i][j] == 0)
            {
                string buffer = vars[j];
                sdnf += '(!';
                sdnf += buffer;
            }
            else
            {
                string buffer = vars[j];
                sdnf+=(buffer);
            }
            if (j != cons[i].size() - 1)
            {
                sdnf += ")/\\";
            }
            else
            {
                //sdnf += ")";
                break;
            }
        }
        if (i != cons.size() - 1)sdnf += "))\\/";
    }
    sdnf.push_back(')');
    return sdnf;
}

string toSKNF(vector<vector<int>> table, vector<string> vars)
{
    string sdnf;
    vector<vector<int>> cons;
    vector<int> buffer;
    int table_size = pow(2, vars.size());
    for (int i = 0; i < table_size; i++)
    {
        if ((table[i][table[i].size() - 1]) == 0)
        {
            for (int j = 0; j < vars.size(); j++)
            {
                buffer.push_back(table[i][j]);
            }
            cons.push_back(buffer);
            buffer.clear();
        }
    }
    for (int i = 0; i < cons.size(); i++)
    {
        sdnf.push_back('(');
        for (int j = 0; j < cons[i].size(); j++)
        {   
            sdnf.push_back('(');

            if (cons[i][j] == 1)
            {
                string buffer = vars[j];
                sdnf += '(!';
                sdnf += buffer;
            }
            else
            {
                string buffer = vars[j];
                sdnf += (buffer);
            }
            if (j != cons[i].size() - 1)
            {
                sdnf += ")\\/";
            }
            else
            {
                //sdnf += ")";
                break;
            }

        }
        if (i != cons.size() - 1)sdnf += "))/\\";

    }
    sdnf.push_back(')');
    return sdnf;
}

int main()
{
    //(((A\/B)/\(F\/S))->((C\/D)/\(!Q)))
    //(!(A\/(!B))/\(!((!A)/\(!B))))
    setlocale(LC_ALL, "rus");
    vector<int> binary, digit_form_d, digit_form_k;
    string formula;
    cout << "Формула: ";
    cin >> formula;
    int vars = getVariables(formula).size();
    int table_size = pow(2, vars);
    vector<vector<int>> table = createTruthTabe(formula);
    for (int i = 0; i < table_size; i++)
        binary.push_back(table[i][table[i].size() - 1]);
    for (int i = 0; i < table_size; i++)
    {
        if (binary[i] == 1)digit_form_d.push_back(i);
        else digit_form_k.push_back(i);
    }
    cout << "\nИндексная форма: " << binaryToDecimal(binary) << endl;;
    cout << "\nЧисловая форма: ";
    cout << "\nСДНФ:";
    for (int i = 0; i < digit_form_d.size(); i++)
    {
        cout << digit_form_d[i]<<" ";
    }
    cout << "\n\nСKНФ:";
    for (int i = 0; i < digit_form_k.size(); i++)
    {
        cout << digit_form_k[i] << " ";
    }
    cout << "\n\nВ СДНФ: ";
    cout<<toSDNF(table, getVariables(formula));
    cout << "\n\nВ СKНФ: ";
    cout<<toSKNF(table, getVariables(formula));




    //В СДНФ : ((!A)/\(B))

    //В СKНФ : ((A)\/(B)))/\((!A)\/(B)))/\((!A)\/(!B))
}
//((Q\/W)/\(E/\R)/\(T\/Y)/\(U/\I)/\(O/\P)/\(A/\S)/\(D/\F)/\(G/\H)\/(J/\K)/\(L/\Z)/\(X/\C)/\(V/\B)/\(N/\M))