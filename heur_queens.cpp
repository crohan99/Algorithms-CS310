/**
 *  heur_queens.cpp
 *  nQueens
 *
 *  Created by Carson Rohan on 12/4/19.
 *  Copyright © 2019 Carson Rohan. All rights reserved.
 *
 *  This program will heuristically solve the n queens problem and display a
 *  virtual chess board with the queen placements on it.
 */

#include <stdio.h>
#include <cassert>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
using namespace std;

void hr(unsigned cols);
void print_board(const vector<unsigned>& perm);
unsigned count_collisions(const vector<unsigned>& perm,
                          unsigned& total_comparisons);
void run_heuristic(vector<unsigned>& perm, unsigned& resets,
                   unsigned& total_swaps, unsigned& total_comparisons);

/**
 *  Set up the vector and begin the testing with a random permutation of the
 *  board
 */
int main(int argc, char* argv[])
{
  // This will seed random_shuffle
  srand(static_cast<unsigned>(time(nullptr)));
  
  // Due to issues with seeding on Macintosh machines,
  // this is a temporary solution when paired with shuffle()
  // instead of random_shuffle()
  random_device rd;
  mt19937 g(rd());
  
  if (argc != 2)
  {
    cout << "Usage: " << argv[0] << " n" << endl;
    cout << "       where n is the number of queens to place" << endl;
    cout << "       on an n x n chessboard, with 4 <= n < 26" << endl;
    exit(2);
  }

  unsigned n = static_cast<unsigned>(stoul(argv[1]));
  assert(n > 3 && n < 26);
  
  unsigned total_comparisons {0};
  unsigned resets {0};
  unsigned total_swaps {0};
  
  // Initialize a vector of size n queens with positions 1..n
  vector<unsigned> perm;
  
  for(unsigned i {0}; i < n; i++)
  {
    perm.push_back(i + 1);
  }
  
  // This is a part of the temporary solution when random_shuffle() would
  // not seed
  shuffle(perm.begin(), perm.end(), g);

  // Run the first permutation
//  random_shuffle(perm.begin(), perm.end());
  
  run_heuristic(perm, resets, total_swaps, total_comparisons);
  cout << "Performed " << resets << " resets, " << total_swaps << " swaps, and "
  << total_comparisons << " comparisons." << endl;
  print_board(perm);
}

/**
 *  A simple procedure to output an ASCII art horizontal line with plus
 *  signs where columns will intersect
 */
void hr(unsigned cols)
{
  cout << "    +";
  for (unsigned col = 0; col < cols; col++)
  {
    cout << "---+";
  }
  cout << endl;
}

/**
 *  A method to print out an nxn chess board with the queens on it displayed
 *  as X's
 *
 *  @param perm the vector by which we place the queens
 */
void print_board(const vector<unsigned>& perm)
{
  hr(static_cast<unsigned>(perm.size()));
  for (unsigned col = 0; col < perm.size(); col++)
  {
    cout << ' ' << setw(2) << perm.size() - col << " |";
    for (unsigned row = 0; row < perm.size(); row++)
    {
      if (perm.at(col) == row + 1)
      {
        cout << " X |";
      }
      else
      {
        cout << "   |";
      }
    }
    cout << endl;
    hr(static_cast<unsigned>(perm.size()));
  }

  cout << "     ";
  for (unsigned col = 0; col < perm.size(); col++)
  {
    cout << ' ' << static_cast<char>('a' + col) << "  ";
  }
  cout << endl;
}

/**
 *  Counts the collisions of each queen on the board and returns it
 *
 *  @param perm the vector of queen placements
 *  @return the number of collisions
 */
unsigned count_collisions(const vector<unsigned>& perm,
                          unsigned& total_comparisons)
{
  unsigned n {static_cast<unsigned>(perm.size())};
  unsigned collisions {0};
  
  for (unsigned col = 0; col < n; col++)
  {
    for(unsigned trycol = col; trycol < n; trycol++)
    {
      // each number in the vector must be unique, so don't have to check for
      // non-diagonal collisions
      if(col != trycol)
      {
        // check for down diag queen
        if(n - col + perm.at(col) == n - trycol + perm.at(trycol))
        {
          collisions++;
          total_comparisons++;
        }
        // check for up diag queen
        if(perm.at(col) + col + 1 == perm.at(trycol) + trycol + 1)
        {
          collisions++;
          total_comparisons++;
        }
      }
    }
  }
  return collisions;
}

/**
 *  Runs the heuristic algorithm on a vector of queen placements, then prints
 *  the board, the number of resets, swaps, and comparisons
 *
 *  @param perm the vector of queen placements
 */
void run_heuristic(vector<unsigned>& perm, unsigned& resets,
                   unsigned& total_swaps, unsigned& total_comparisons)
{
  unsigned swaps;
  unsigned collisions;
  vector<unsigned> test_swap(perm);
  
  do
  {
    random_shuffle(perm.begin(), perm.end());
    resets++;
    do
    {
      swaps = 0;
      for(unsigned i = 0; i < perm.size(); i++)
      {
        for(unsigned j = i; j < perm.size(); j++)
        {
          swap(test_swap.at(i), test_swap.at(j));
          
          if(count_collisions(test_swap, total_comparisons) <
             count_collisions(perm, total_comparisons))
          {
            swap(perm.at(i), perm.at(j));
            test_swap = perm;
            swaps++;
            total_swaps++;
          }
        }
      }
    } while(swaps > 0);
    collisions = count_collisions(perm, total_comparisons);
  } while(collisions > 0);
}

