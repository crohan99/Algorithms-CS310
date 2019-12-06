/**
 * calculate the optimum alignment between two strings
 * @author Carson Rohan
 * @version Oct 28, 2019
 */
#include <climits>
#include <iomanip>
#include <iostream>
#include "matrix.h"
using namespace std;

const int INF {INT_MAX};

/**
 * find the maximum of three values
 * @param a one of the three values
 * @param b one of the three values
 * @param c one of the three values
 * @return the largest value
 */
int max3(int a, int b, int c);


/**
 * the recursive, memoized optimum function for computing the edit distance
 * between two strings
 * @param s the first string
 * @param i the index of the first string
 * @param t the second string
 * @param j the index of the second string
 * @param memo the memo table
 * @return the optimal alignment score of the two strings
 */
int opt(const string& s, size_t i, const string& t, size_t j,
        Matrix<int>& memo, int match, int mismatch, int gap);


/**
 * find and print the optimal alignment of the two strings
 * @param memo the memo table
 * @param s the first string
 * @param t the second string
 * @param gap the gap penalty
 */
void traceback(const Matrix<int>& memo, const string& s, const string& t,
               int gap);

/**
 * dump the memo table to standard output
 * @param memo the memo table
 * @param s the first string
 * @param t the second string
 */
void print_memo(const Matrix<int>& memo, const string& s, const string& t);


int main(int argc, char* argv[])
{
  if (argc != 6)
  {
    cerr << "Usage: " << argv[0] << " s1 s2 match mismatch gap" << endl;
    return 1;
  }
  
  // add a space to the beginning of each string to allow traceback
  string s {argv[1]};
  s = ' ' + s;
  string t {argv[2]};
  t = ' ' + t;
  
  int match {stoi(argv[3])};
  int mismatch {stoi(argv[4])};
  int gap {stoi(argv[5])};
  
  cout << "match: " << match << endl;
  cout << "mismatch: " << mismatch << endl;
  cout << "gap: " << gap << endl;
  
  Matrix<int> memo(s.size() + 1, t.size() + 1);
  for (size_t row {0}; row <= s.size(); row++)
  {
    for (size_t col {0}; col <= t.size(); col++)
    {
      memo.at(row, col) = INF;
    }
  }
  
  int score
  {opt(s, s.size() - 1, t, t.size() - 1, memo, match, mismatch, gap)};
  
  cout << "The optimal alignment score between " << s << " and " << t <<
  " is " << score << endl;
  
  cout << endl << "The completed memo table: " << endl << endl;
  
  print_memo(memo, s, t);
  
  traceback(memo, s, t, gap);
  return 0;
}

void print_memo(const Matrix<int>& memo, const string& s, const string& t)
{
  int field_width {6};
  int left_label_width {6};
  int left_index_width {3};
  
  cout << right << setw(left_label_width) << ' ';
  for (size_t col {0}; col < t.size(); col++)
  {
    cout << setw(field_width) << t.at(col);
  }
  cout << endl;
  
  cout << setw(left_label_width) << ' ';
  for (size_t col {0}; col < t.size(); col++)
  {
    cout << setw(field_width) << col;
  }
  cout << endl;
  
  cout << setw(left_label_width) << '+';
  for (size_t col {0}; col < t.size(); col++)
  {
    cout << setw(field_width) << "---";
  }
  cout << endl;
  
  for (size_t row {0}; row < s.size(); row++)
  {
    cout << s.at(row) << setw(left_index_width) << row << " |";
    for (size_t col {0}; col < t.size(); col++)
    {
      if (memo.at(row, col) == INF)
      {
        cout << setw(field_width) << "inf";
      }
      else
      {
        cout << setw(field_width) << memo.at(row, col);
      }
    }
    cout << endl;
  }
}

int max3(int a, int b, int c)
{
  int result {a > b ? a : b};
  return result > c ? result : c;
}

int opt(const string& s, size_t i, const string& t, size_t j,
        Matrix<int>& memo, int match, int mismatch, int gap)
{
  // the type of score according to match or mismatch
  int score_type {0};
  
  if (memo.at(i, j) == INF)
  {
    // if the letters are the same: match. Else, mismatch
    if (s.at(i) == t.at(j))
    {
      score_type = match;
    }
    else
    {
      score_type = mismatch;
    }
    // if both index are at 0, then both are gaps and that index should be 0
    if (i == 0 && j == 0)
    {
      memo.at(i, j) = 0;
    }
    // gap compared to every letter in the columns
    else if (i == 0 && j != 0)
    {
      memo.at(i, j) = opt(s, i, t, j - 1, memo, match, mismatch, gap) + gap;
    }
    // gap compared to every letter in the rows
    else if (j == 0 && i != 0)
    {
      memo.at(i, j) = opt(s, i - 1, t, j, memo, match, mismatch, gap) + gap;
    }
    // comparing the letters
    else
    {
      memo.at(i, j) = max3(
                opt(s, i-1, t, j-1, memo, match, mismatch, gap) + score_type,
                opt(s, i-1, t, j, memo, match, mismatch, gap) + gap,
                opt(s, i, t, j-1, memo, match, mismatch, gap) + gap);
    }
  }
  return memo.at(i, j);
}

void traceback(const Matrix<int>& memo, const string& s, const string& t,
               int gap)
{
  string optimal_s {""};
  string optimal_t {""};
  size_t row {s.size() - 1};
  size_t col {t.size() - 1};
  
  // while rows and cols > 0
  while (row > 0 && col > 0)
  {
    // if this index came from top, put gap in top
    if ((memo.at(row - 1, col) + gap) == memo.at(row, col))
    {
      optimal_t += "-";
      optimal_s += s.at(row);
      row--;
    }
    // if this index came from left, put gap in left
    else if ((memo.at(row, col - 1) + gap) == memo.at(row, col))
    {
      optimal_s += "-";
      optimal_t += t.at(col);
      col--;
    }
    // else this index came from diagonal, so add both letters
    else
    {
      optimal_s += s.at(row);
      optimal_t += t.at(col);
      row--;
      col--;
    }
  }
  // reverse both strings
  reverse(optimal_s.begin(), optimal_s.end());
  reverse(optimal_t.begin(), optimal_t.end());
  
  cout << endl;
  cout << "The aligned strings:" << endl;
  cout << optimal_s << endl;
  cout << optimal_t << endl;
}
