#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <math.h>
#include <map>
#include <set>

using namespace std;

struct comp {
    template <typename T>

    // Comparator function
    bool operator()(const T& l,
        const T& r) const
    {
        if (l.second != r.second) {
            return l.second < r.second;
        }
        return l.first < r.first;
    }
};

vector<vector<double>> sortM(map<int, double>& M)
{
    vector<vector<double>> ans;

    // Declare set of pairs and insert
    // pairs according to the comparator
    // function comp()
    set<pair<int, double>, comp> S(M.begin(),
        M.end());

    // Print the sorted value
   for (auto& it : S) {
       vector<double> temp;
       temp.push_back(it.first);
       temp.push_back(it.second);
       ans.push_back(temp); 
    }
   return ans;
}



class Customer {
public:
    int cap;
    int appoint;
    double x, y;
    bool taken;
    Customer(int c, double x1, double y1) { // constructor
        cap = c;
        x = x1;
        y = y1;
        taken = false;
    }
    int returnDemand() { // returning demand
        return cap;
    }
    void served() { //change the cutomer to be taken
        taken = true;
    }
    void appointFac(int f) {
        appoint = f;
    }
};


class Facility {
public: 
    int cap;
    int id;
    double cost;
    double x, y;
    bool operates; 
    map<int, double> serve_list;
    Facility(double c, int cp, double x1, double y1, int idd) { //constructor
        cost = c;
        cap = cp;
        x = x1;
        y = y1;
        id = idd;
        operates = false;
    }
    double calcCost(Customer* ct) {
        double distance = sqrt(pow((ct->x - x), 2) + pow((ct->y - y), 2));
        return distance;
    }
    bool operator < (const Facility fac) const {
        return (cost < fac.cost);
    }

};


vector<int> Greedy(vector<Facility> fac_list, vector<Customer*> cus_list, int f_size, int c_size, double &cst) {
    //sort the facility list by setup cost in ascending order
    sort(fac_list.begin(), fac_list.end());
    vector<int> ans;
    //for each facilities in the fac_list
    double cost = 0;
    for (int i = 0; i < f_size; i++) {
        for (int j = 0; j < c_size; j++) {
            if (cus_list[j]->taken == false) {
                fac_list[i].serve_list[j] = fac_list[i].calcCost(cus_list[j]);
            }
        }
        //sort the distance
        vector<vector<double>> sort_list = sortM(fac_list[i].serve_list);
        for (int j = 0; j < sort_list.size(); j++) {
            int ii = sort_list[j][0]; 
            int c_cap = fac_list[i].cap - cus_list[ii]->cap;
            if (c_cap >= 0) {
                fac_list[i].operates = true;
                cus_list[ii]->served();
                cus_list[ii]->appointFac(fac_list[i].id);
                fac_list[i].cap = c_cap;
                cost += sort_list[j][1];
            }
        }
        if (fac_list[i].operates == true) cost += fac_list[i].cost;
    }
    for (int i = 0; i < c_size; i++) {
        ans.push_back(cus_list[i]->appoint);
    }
    cst = cost;
    return ans;
}


int main()
{
    cout << "*****Facility Location******" << endl;
    string filenames = "filenames";
    ifstream iinfile;
    iinfile.open(filenames);
    string s;
    iinfile >> s;
    iinfile.close();
    int ssize = s.size();
    while (s.size() != 0) {
        int init = s.find(",");
        string filename = "fl_" + s.substr(0, init);
        cout << filename << endl;
        ifstream infile;
        infile.open("input/" + filename);
        if (!infile.is_open()) {
            cout << "Can't open this file!" << endl << endl;
        }
        //at this point have successfully read the file
        //get the first line of the text file
        int f_size, c_size, i;
        if (infile >> i) {
            f_size = i;
            infile >> i;
            c_size = i;
        }
        int count = 0;
        vector<Facility> fac_list;
        vector<Customer*> cus_list;
        int cap;
        double x, y, cost;
        int fc_id = 0;
        while (infile >> cost && count < f_size) {
            infile >> cap;
            infile >> x;
            infile >> y;
            Facility fc = Facility(cost, cap, x, y, fc_id);
            fac_list.push_back(fc);
            count++;
            fc_id++;
        }
        count = 1;
        infile >> x;
        infile >> y;
        Customer* ct = new Customer(cost, x, y);
        cus_list.push_back(ct);
        int i1;
        while (infile >> i1 && count < c_size) {
            infile >> x;
            infile >> y;
            Customer* ct = new Customer(i1, x, y);
            cus_list.push_back(ct);
            count++;
        }
        infile.close();
        //at this point we have facility infor at 
        //fac_list and customer info at cus_list
        double cst = 0;
        vector<int> ans = Greedy(fac_list, cus_list, f_size, c_size, cst);
        ofstream output(filename + "_output");
        output << cst << endl;
        for (int i = 0; i < ans.size(); i++) {
            output << ans[i] << " ";
        }
        output.close();
        s = s.substr(init + 1, s.size() - init);
    }
}

