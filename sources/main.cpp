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

void  examine_caves_in_sequence(vector<int> &sequence, int nb_cylinders, bool reverse_order) {
    double  sum_distance = 0;
    for (int i = 0; i < (int)sequence.size() - 1; i++) {
        for (int j = i; j < min(i + 1 + nb_cylinders, (int)sequence.size() - 1); j++) {
            sum_distance += distance(sequence[j], sequence[j + 1]);
            if (sum_distance < distance(sequence[i], sequence[j + 1])) {
                for (int k = i; k < j; k++)
                    sequence.erase(sequence.begin() + i + 1);
                break;
            }
        }
        sum_distance = 0;
    }
    if (nb_cylinders > 1)
        examine_caves_in_sequence(sequence, nb_cylinders - 1, false);
    reverse(sequence.begin(), sequence.end());
    if (!reverse_order)
        examine_caves_in_sequence(sequence, nb_cylinders, true);
}


void  examine_sequence_front(vector<int> &sequence, bool reverse_order) {
    double  sum_distance = sequence[0];
    for (int i = 0; i < (int)sequence.size() - 1; i++) {
        sum_distance += distance(sequence[i], sequence[i + 1]);
        if (sum_distance < sequence[i + 1]) {
            for (int j = 0; j < i + 1; j++)
                sequence.erase(sequence.begin());
            sum_distance = sequence[0];
            i = - 1;
        }
    }
    reverse(sequence.begin(), sequence.end());
    if (!reverse_order)
        examine_sequence_front(sequence, true);
}

double  compute_distance(const vector<int> &sequence) {
    double total_distance = 0;
    vector<int> new_sequence(sequence);
    int size;

    examine_caves_in_sequence(new_sequence, 1, false);
    examine_caves_in_sequence(new_sequence, 2, false);
    examine_caves_in_sequence(new_sequence, 3, false);
    examine_sequence_front(new_sequence, false);
    size = (int)new_sequence.size();
    for (int i = 1; i < size; i++)
        total_distance += distance(new_sequence[i - 1], new_sequence[i]);
    total_distance += new_sequence[0] + new_sequence[size - 1];
    return (total_distance);
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

    //cerr << "size sequence : " << size << endl;
    while (position < size - 1) {
        position++;
        order[position] = position;
    }
    order_sequence(sequence, order);
    min_dist = compute_distance(sequence);
    while (!next_order(*original_sequence, order)) {
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
    //ex: Example
    //int test[8] = { 2, 1, 2 };
    //res = 9.657;
    //int test[8] = { 3, 3, 3, 3 };
    //res = 24.000;
    //int test[8] = { 900, 970, 567, 965 };
    //res = 6696.314;
    //int test[8] = { 3, 7, 380, 150, 93, 542, 135, 10 };
    //res = 2163.508
    //int test[8] = { 13, 8, 2, 1, 3, 1, 5, 2 }
    //res = 53.811

    //ex: Caves to fill in
    //9
    //1 500
    //1000.000
    //2 5 500
    //1000.000
    //3 5 10 15
    //56.463
    //4 20 1 2 3
    //40.000
    //5 2 3 2 3 3
    //25.596
    //6 121 6 3 12 28 17
    //265.413
    //7 60 70 80 10 30 40 300
    //*886.430
    //8 4137 183 51 96 109 99 183 10
    //*8274.000
    //int test[9] = { 3, 4, 3, 3, 200, 2, 400, 4, 11 }
    //*800.000

    //ex: Harsh
    //10
    //1 687
    //1374.000
    //2 580 431
    //2010.960
    //3 326 14 549
    //1721.106
    //4 518 47 784 308
    //3083.334
    //5 114 473 562 318 573
    //3775.436
    //6 985 305 540 647 367 866
    //6855.840
    //6 135 13 57 943 900 9
    //3685.498
    //7 914 398 108 36 688 846 231
    //5589.961
    //8 739 211 458 965 253 492 587 174
    //6719.488
    //9 279 496 796 644 602 765 723 715 85

    //ex: Optimize your code
    //10
    //5 677 976 287 977 213
    //5442.404
    //int test[9] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 }
    //*80.103
    //8 287 326 905 539 993 159 665 152
    //4 180 574 839 223
    //5 481 947 358 911 426
    //9 442 721 165 672 157 346 696 767 269
    //5 377 250 727 905 452
    //7 432 647 785 609 875 339 76

    
//    vector<int> vec_test;
//    int test[9] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    //vec_test.push_back(0);
//    for (int i = 0; i < 9; i++) {
//       vec_test.push_back(test[i]);
//    }
    //vec_test.push_back(0);
//    sort(vec_test.begin(), vec_test.end());
    //printVector(vec_test);
//    cout << setprecision(3) << fixed << min_distance(vec_test) << endl;

    //examine_sequence(vec_test);
    //vector<double> distances = compute_sequence_distances(vec_test);
    //printVectorDouble(distances);
    //cerr << "distance = " << add_distances(distances) << endl;
//    return 0;
    int n;
    struct timeval start;
    struct timeval tp;
    double  distance = 0;
    vector<int> numbers;
    string line;

    cin >> n; cin.ignore();
//    cerr << n << endl;
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
        cerr << "step[" << i << "] : " << tp.tv_sec * 1000 + tp.tv_usec / 1000 - start.tv_sec * 1000 + start.tv_usec / 1000 << " ms" << endl;
        numbers.clear();
    }
    //cout << 0.000 << endl;


    // Write an answer using cout. DON'T FORGET THE "<< endl"
    // To debug: cerr << "Debug messages..." << endl;

    //cout << "answer" << endl;
}