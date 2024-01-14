#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
#include <sys/time.h>

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

typedef struct s_cylinder
{
    double  position;
    int     radius;
}   t_cylinder;

double  distance(double r1, double r2) {
    if (!(r1 * r2))
        return (max(r1, r2));
    return (2 * sqrt(r1 * r2));
}

vector<int> split(string str) {
    int i = 0;
    int j = 0;
    std::string  word;
    vector<int>  res;

    while (i < (int)str.size()) {
        j = i;
        while (str[i] != ' ' && str[i] != '\0') {
            i++;
        }
        word = str.substr(j, i - j);
        //cerr << "word : " << word;
        res.push_back(stoi(word));
        i++;
    }
    return (res);
}

bool    isUsed(int rank, const vector<int> &order, int position) {
    int i = 0;

    while (i < position) {
        if (rank == order[i])
            return (true);
        i++;
    }
    return (false);
}

int next_order(const vector<int> &sequence, vector<int> &order) {
    int size = (int)order.size();
    int position = size - 1;
    int candidate;
    int curCandidate;

    while (position >= 0) {
        curCandidate = order[position];
        candidate = curCandidate;
        while (++candidate < size && (isUsed(candidate, order, position) || sequence[candidate] == sequence[curCandidate]))
                ;
        if (candidate > size - 1)
            position--;
        else {
            order[position] = candidate;
            break;
        }
    }
    if (position == -1)
        return (1);
    else {
        while (++position < size) {
            candidate = -1;
            while (++candidate < size && isUsed(candidate, order, position))
                ;
            order[position] = candidate;
        }
    }
    return (0);
}

void    printVector(const vector<int> vec) {
    cerr << "Seq : ";
    
    for (int i = 0; i < (int)vec.size(); i++) {
        cerr << vec[i] << " ";
    }
    cerr << endl;
}

void    printVectorDouble(const vector<double> vec) {
    cerr << "Seq : ";
    
    for (int i = 0; i < (int)vec.size(); i++) {
        cerr << vec[i] << " ";
    }
    cerr << endl;
}

void    compute_positions(vector<t_cylinder> &positions, int n = 1) {
    double most_right_position = 0;
    double calc_position = 0;

    if (n == (int)positions.size()) {
        return ;
    }
    //cerr << "       calculating position : " << n << " radius : " << positions[n].radius << endl;
    for (int i = 0; i < n; i++) {
        calc_position = positions[i].position + distance(positions[i].radius, positions[n].radius);
        //cerr << "       : " << i << " (" << positions[i].position << " + " << distance(positions[i].radius, positions[n].radius) << " = " << calc_position << ")" << endl;
        if (most_right_position == 0 || calc_position > most_right_position) {
            //cerr << "       *** updating" << endl;
            most_right_position = calc_position;
        }
    }
    //cerr << "       *** final : " << most_right_position << endl;
    positions[n].position = most_right_position;
    compute_positions(positions, n + 1);
}

double  compute_distance(const vector<int> &sequence) {
    int     size = (int)sequence.size();
    vector<t_cylinder>    positions; 
    t_cylinder cyl;

    // add left wall
    cyl.position = 0.0;
    cyl.radius = 0;
    positions.push_back(cyl);
    for (int i = 0; i < size; i++) {
        cyl.position = 0.0;
        cyl.radius = sequence[i];
        positions.push_back(cyl);
    }
    // add right wall
    cyl.position = 0.0;
    cyl.radius = 0;
    positions.push_back(cyl);

    compute_positions(positions);
    //cerr << positions[size + 2].position << endl;
    return(positions[size + 1].position);
}

void order_sequence(vector<int> &sequence, const vector<int> &order) {
    vector<int> temp(sequence);
    int idx = -1;

    for (int i = 0; i < (int)sequence.size(); i++) {
        idx = order[i];
        sequence[i] = temp[idx];
    }
}

int factorial (int n) {
    if (n == 1)
        return 1;
    else
        return (n * factorial(n - 1));
}

double min_distance(vector<int> *original_sequence) {
    int size = (int)(*original_sequence).size();
    vector<int> order(size, -1);
    vector<int> sequence(*original_sequence);
    int position = -1;
    double min_dist = 0;
    double cur_dist = 0;
    int count = 0;

    //cerr << "size sequence : " << size << endl;
    while (position < size - 1) {
        position++;
        order[position] = position;
    }
    order_sequence(sequence, order);
    //printVector(sequence);
    min_dist = compute_distance(sequence);
    while (count++ < factorial(size) / 1 && !next_order(*original_sequence, order)) {
        sequence = *original_sequence;
        order_sequence(sequence, order);
        cur_dist = compute_distance(sequence);
        if (cur_dist < min_dist)
            min_dist = cur_dist;
    }
    order.clear();
    return (min_dist);
}

int main()
{ 
    int n;
    struct timeval start;
    struct timeval tp;
    double  distance = 0;
    vector<int> numbers;
    string line;

    cin >> n; cin.ignore();
    for (int i = 0; i < n; i++) {
        getline(cin, line);
        cerr << line << endl;
        numbers.clear();
        numbers = split(line);
        numbers.erase(numbers.begin());
        sort(numbers.begin(), numbers.end());
        //cerr << "size numbers : " << (int)numbers.size() << endl;
        //printVector(numbers);
        if ((int)numbers.size() == 1)
            distance = 2.0 * numbers[0];
        else
            distance = min_distance(&numbers);
        cout << setprecision(3) << fixed << distance << endl;
        gettimeofday(&start, NULL);
        gettimeofday(&tp, NULL);
        //cerr << "step[" << i << "] : " << tp.tv_sec * 1000 + tp.tv_usec / 1000 - start.tv_sec * 1000 + start.tv_usec / 1000 << " ms" << endl;
        numbers.clear();
    }
    // Write an answer using cout. DON'T FORGET THE "<< endl"
    // To debug: cerr << "Debug messages..." << endl;

    //cout << "answer" << endl;
}