#include <algorithm>
#include <limits>
#include "kmeans.h"

kmeans::kmeans(Dataset *d, int NumberOfTeams) {
    dataset = d;
    teams = NumberOfTeams;
    dimension = d->cols();
    centroid.resize(teams);
    members.resize(dataset->rows());
    for (int i = 0; i < members.size(); i++)
        members[i] = 0;

    for (int i = 0; i < centroid.size(); i++)
        centroid[i].resize(d->cols());
    indexes.resize(teams);
    srand(time(0));
    for (int i = 0; i < indexes.size(); i++)
        indexes[i] = (0 + rand() % d->rows());
    TestCoord.resize(teams);
    for (int i = 0; i < teams; i++)
        TestCoord[i].resize(dimension);
    for (int i = 0; i < TestCoord.size(); i++) {
        int index = indexes.at(i);
        TestCoord[i] = d->getX(index);
    }

    for (int i = 0; i < centroid.size(); i++) {
        centroid[i] = TestCoord[i];
    }
}

double kmeans::distance(Data &x, Data &y) {
    double s = 0.0;
    int i;
    for (i = 0; i < x.size(); i++)
        s += pow(x[i] - y[i], 2.0);
    return sqrt(s);
}

int kmeans::nearestTeam(Data &x) {
    int i;
    double minDist = std::numeric_limits<double>::max();
    int min = -1;
    for (i = 0; i < teams; i++) {
        double dist = distance(x, centroid[i]);
        if (dist < minDist) {
            minDist = dist;
            min = i;
        }
    }
    return min;
}

void kmeans::AssignTeams() {
    NumberOfEach.resize(teams);
    for (int i = 0; i < NumberOfEach.size(); i++)
        NumberOfEach.at(i) = 0;
    for (int i = 0; i < dataset->rows(); i++) {
        Data x = dataset->getX(i);
        int t = nearestTeam(x);
        members[i] = t;
    }
    for (int i = 0; i < members.size(); i++) {
        NumberOfEach[members[i]]++;
    }
}

void kmeans::CalculateMean() {
    for (int i = 0; i < centroid.size(); i++) {
        for (int j = 0; j < centroid[i].size(); j++)
            centroid[i][j] = 0;

    }
    for (int i = 0; i < dataset->rows(); i++) {
        Data x = dataset->getX(i);
        int index = members[i];
        int size = NumberOfEach[index];
        for (int j = 0; j < dataset->cols(); j++) {
            centroid[index][j] += x[j] / size;
        }
    }
}


//compare function
bool kmeans::compare(Matrix A, Matrix B) {
    for (int i = 0; i < A.size(); i++) {
        for (int j = 0; j < A[i].size(); j++) {
            if (std::abs(A[i][j] - B[i][j]) <= std::numeric_limits<double>::min()) //den uparxoun
                return true;
        }
    }
    return false;
}

void kmeans::run() {

    Matrix copy;
    AssignTeams();

    while (1) {

        copy = centroid;
        AssignTeams();
        CalculateMean();
        if (compare(centroid, copy))
            break;

    }
}

Data kmeans::getVariance() {
    return variance;
}

Matrix kmeans::getCenters() {
    return centroid;
}

void kmeans::calculateVariance() {
    variance.resize(teams);
    for (int i = 0; i < variance.size(); i++)
        variance[i] = 0;
    vector<double> sum;
    vector<double>::iterator it;
    for (int i = 0; i < centroid.size(); ++i) {
        for (int j = 0; j < centroid.size(); ++j) {
            double result = distance(centroid[i], centroid[j]);
            it = find(sum.begin(), sum.end(), result);
            if (it == sum.end())
                sum.push_back(result);
        }
    }
    double sumResult = 0;
    for (int i = 0; i < sum.size(); i++)
        sumResult += sum[i];

    sumResult /= (double) teams;

    for (int i = 0; i < variance.size(); i++)
        variance[i] = 2 * sumResult;
}

void kmeans::CalculateVariance() {
    variance.resize(teams);
    for (int i = 0; i < variance.size(); i++)
        variance[i] = 0;
    double sum;
    double Nteams = (double) teams;
    for (int i = 0; i < centroid.size(); i++) {
        sum = 0;
        for (int j = 0; j < centroid.size(); j++)
            sum += distance(centroid[i], centroid[j]);
        double result = (1 / Nteams) * sum;
        variance[i] = sqrt(result);
    }
}

void kmeans::calculateGenVariance() {

    variance.resize(teams);
    for (int i = 0; i < variance.size(); i++)
        variance[i] = 0;

    vector<double> sum;
    vector<double>::iterator it;

    for (int i = 0; i < centroid.size(); ++i) {
        for (int j = 0; j < centroid.size(); ++j) {
            double result = distance(centroid[i], centroid[j]);
            it = find(sum.begin(), sum.end(), result);
            if (it == sum.end())
                sum.push_back(result);
        }
    }

    double max_spacing_d = *max_element(sum.begin(), sum.end());

    double num = sqrt(2 * teams);

    for (int i = 0; i < variance.size(); i++)
        variance[i] = max_spacing_d / num;

}
