#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
using namespace std;

int rulenum = 15;
int ca[256][15];
double length = 256.0;

int power(int a, int b)
{
    int result = 1;
    for (int i = 0; i < b; i++)
    {
        result = a * result;
    }
    return result;
}

int upgradecell(int rule, int left, int middle, int right)
{
    int newstate[8];
    newstate[7] = rule / 128;
    newstate[6] = (rule % 128) / 64;
    newstate[5] = (rule % 64) / 32;
    newstate[4] = (rule % 32) / 16;
    newstate[3] = (rule % 16) / 8;
    newstate[2] = (rule % 8) / 4;
    newstate[1] = (rule % 4) / 2;
    newstate[0] = rule % 2;
    if (left == 0 and middle == 0 and right == 0)
        return newstate[0];
    if (left == 0 and middle == 0 and right == 1)
        return newstate[1];
    if (left == 0 and middle == 1 and right == 0)
        return newstate[2];
    if (left == 0 and middle == 1 and right == 1)
        return newstate[3];
    if (left == 1 and middle == 0 and right == 0)
        return newstate[4];
    if (left == 1 and middle == 0 and right == 1)
        return newstate[5];
    if (left == 1 and middle == 1 and right == 0)
        return newstate[6];
    if (left == 1 and middle == 1 and right == 1)
        return newstate[7];
}

double entropy(int i)
{
    int num[16] = {0};
    double sum = 0;
    for (int j = 0; j < length; j = j + 4)
    {
        int temp;
        temp = ca[j][i] * 8 + ca[j + 1][i] * 4 + ca[j + 2][i] * 2 + ca[j + 3][i];
        num[temp] = num[temp] + 1;
    }
    for (int k = 0; k < 16; k++)
    {
        if (num[k] != 0)
        {
            sum = sum + (num[k] / (length / 4)) * (log (num[k] / (length / 4)) / log(2));
        }
    }
    return (-1) * sum;
}

int mutation(int rule)
{
    int sum = 0;
    int newstate[8];
    newstate[7] = rule / 128;
    newstate[6] = (rule % 128) / 64;
    newstate[5] = (rule % 64) / 32;
    newstate[4] = (rule % 32) / 16;
    newstate[3] = (rule % 16) / 8;
    newstate[2] = (rule % 8) / 4;
    newstate[1] = (rule % 4) / 2;
    newstate[0] = rule % 2;
    if (rand() % 6 == 0)
    {
        int mut = rand() % 8;
        newstate[mut] = 1 - newstate[mut];
    }
    for (int i = 0; i < 8; i++)
    {
        sum = sum + newstate[i] * (power(2, i));
    }
    return sum;
}

int upgraderule(int l_rule, int m_rule, int r_rule, double l_entropy, double m_entropy, double r_entropy)
{
    int breakpoint;
    int newrule[8];
    int sum = 0;
    if (m_entropy >= l_entropy and m_entropy >= r_entropy)
        return mutation(m_rule);
    if (m_entropy >= l_entropy and m_entropy < r_entropy)
        return mutation(r_rule);
    if (m_entropy < l_entropy and m_entropy >= r_entropy)
        return mutation(l_rule);
    if (m_entropy < l_entropy and m_entropy < r_entropy)
    {
        breakpoint = rand() % 9;
        for (int i = 0; i < breakpoint; i++)
        {
            newrule[i] = (l_rule % (power(2, (i + 1))))/ (power(2, i));
        }
        for (int i = breakpoint; i < 8; i++)
        {
            newrule[i] = (r_rule % (power(2, (i + 1))))/ (power(2, i));
        }
        for (int i = 0; i < 8; i++)
        {
            sum = sum + newrule[i] * (power(2, i));
        }
        return mutation(sum);
    }
}

int main()
{
    srand(time(NULL));
    int ruleset[rulenum];
    int tempruleset[rulenum];
    cout << "Please enter 15 numbers (0-255):" << endl;
    for (int i = 0; i < rulenum; i++)
    {
        cin >> ruleset[i];
    }

    for (int i = 0; i < 30; i++)
    {
        for (int j = 0; j < rulenum; j++)
        {
            ca[0][j] = rand() % 2;
        }

        for (int j = 0; j < 256; j++)
        {
            for (int i = 0; i < rulenum; i++)
            {
                if (i > 0 and i < rulenum)
                    ca[j + 1][i] = upgradecell(ruleset[i], ca[j][i - 1], ca[j][i], ca[j][i + 1]);
                if (i == 0)
                    ca[j + 1][i] = upgradecell(ruleset[i], ca[j][rulenum - 1], ca[j][i], ca[j][i + 1]);
                if (i == rulenum - 1)
                    ca[j + 1][i] = upgradecell(ruleset[i], ca[j][i - 1], ca[j][i], ca[j][0]);
            }
        }

        cout << "Rule set " << i << " :" << endl;
        for (int i = 0; i < rulenum; i++)
        {
            if (i > 0 and i < rulenum)
                tempruleset[i] = upgraderule(ruleset[i - 1], ruleset[i], ruleset[i + 1], entropy(i - 1), entropy(i), entropy(i + 1));
            if (i == 0)
                tempruleset[i] = upgraderule(ruleset[rulenum - 1], ruleset[i], ruleset[i + 1], entropy(rulenum - 1), entropy(i), entropy(i + 1));
            if (i == rulenum - 1)
                tempruleset[i] = upgraderule(ruleset[i - 1], ruleset[i], ruleset[0], entropy(i - 1), entropy(i), entropy(0));
            cout << tempruleset[i] << "\t" ;
        }

        for (int i = 0; i < rulenum; i++)
        {
            ruleset[i] = tempruleset[i];
        }
        cout << endl;
        cout << endl;
    }
    cout << "Here is the graph of the rule set after 50 generations: " << endl;
    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            cout << ca[i][j] << " ";
        }
        cout << endl;
    }
    system("PAUSE");
    return 0;
}

