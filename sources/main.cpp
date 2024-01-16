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

void    copy_array(int *arr1, int size, int *arr2) {
    for (int i = 0; i < size; i++) {
        arr2[i] = arr1[i];
    }
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

bool    isUsed(int rank, const int *order, int position) {
    int i = 0;

    while (i < position) {
        if (rank == order[i])
            return (true);
        i++;
    }
    return (false);
}

void order_sequence(int *sequence, int size, const int *order) {
    int  temp[size];

    copy_array(sequence, size, temp);
    for (int i = 0; i < size; i++)
        sequence[i] = temp[order[i]];
}

double  distance(double r1, double r2) {
    if (!(r1 * r2))
        return (max(r1, r2));
    return (2 * sqrt(r1 * r2));
}

int next_order(const int *sequence, int *order, int size) {
    int position = size - 1;
    int candidate;
    int curCandidate;

    while (position >= 0) {
        curCandidate = order[position];
        candidate = curCandidate;
        while (++candidate < size && (isUsed(candidate, order, position) || sequence[candidate] == sequence[curCandidate]
            || (candidate == size - 1 && (position < size / 2 || position == size - 1))
            || (candidate > size / 2 + size % 2 && position < size -  1 && position > 0 && (order[position - 1] == candidate - 1))))
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

void    compute_positions(t_cylinder *positions, int size, int n = 1) {
    double most_right_position = 0;
    double calc_position = 0;

    if (n == size)
        return ;
    for (int i = 0; i < n; i++) {
        calc_position = positions[i].position + distance(positions[i].radius, positions[n].radius);
        if (most_right_position == 0 || calc_position > most_right_position)
            most_right_position = calc_position;
    }
    positions[n].position = most_right_position;
    compute_positions(positions, size, n + 1);
}

double  compute_distance(const int *sequence, int size) {
    t_cylinder   positions[size + 2]; 
    t_cylinder cyl;

    cyl.position = 0.0;
    cyl.radius = 0;
    positions[0] = cyl;   
    for (int i = 0; i < size; i++) {
        cyl.position = 0.0;
        cyl.radius = sequence[i];
        positions[i + 1] = cyl;
    }
    cyl.position = 0.0;
    cyl.radius = 0;
    positions[size + 1] = cyl;

    compute_positions(positions, size + 2);
    return(positions[size + 1].position);
}

double min_distance(int *original_sequence, int size) {
    int order[size];
    int sequence[size];
    int position = -1;
    double min_dist = 0;
    double cur_dist = 0;

    copy_array(original_sequence, size, sequence);
    while (position++ < size - 1)
        order[position] = position;
    order_sequence(sequence, size, order);
    min_dist = compute_distance(sequence, size);
    while (!next_order(original_sequence, order, size)) {
        copy_array(original_sequence, size, sequence);
        order_sequence(sequence, size, order);
        cur_dist = compute_distance(sequence, size);
        if (cur_dist < min_dist)
            min_dist = cur_dist;
    }
    return (min_dist);
}

int main()
{ 
    int n;
    double  distance = 0;
    vector<int> numbers;
    string line;

    cin >> n; cin.ignore();
    for (int i = 0; i < n; i++) {
        getline(cin, line);
        numbers.clear();
        numbers = split(line);
        int nb_elem = numbers[0];
        numbers.erase(numbers.begin());
        sort(numbers.begin(), numbers.end());
        int num[nb_elem];
        for (int i = 0; i < nb_elem; i++)
            num[i] = numbers[i];
        if (nb_elem == 1)
            distance = 2.0 * num[0];
        else
            distance = min_distance(num, nb_elem);
        cout << setprecision(3) << fixed << distance << endl;
        numbers.clear();
    }
}