#include <iostream>
#include <vector>
#include <cmath>

const int64_t x = 7;
const int64_t y = 17;
const int64_t p = 1e9 + 7;

std::vector<std::vector<int64_t>> PolynomialHash(std::vector<std::vector<char>>& field) {
  std::vector<std::vector<int64_t>> hashes(field.size(), std::vector<int64_t>(field[0].size(), 0));
  int64_t hash = 0;
  int64_t deg_x = 1;
  for (int i = 0; i < field[0].size(); ++i) {
    hash = (hash + (field[0][i] * deg_x) % p) % p;
    hashes[0][i] = hash;
    deg_x = (deg_x * x) % p;
  }
  hash = 0;
  int64_t deg_y = 1;
  for (int i = 0; i < field.size(); ++i) {
    hash = (hash + (field[i][0] * deg_y) % p) % p;
    hashes[i][0] = hash;
    deg_y = (deg_y * y) % p;
  }
  deg_y = y;
  for (int64_t i = 1; i < hashes.size(); ++i) {
    deg_x = x;
    for (int64_t j = 1; j < hashes[0].size(); ++j) {
      hashes[i][j] = (hashes[i - 1][j] + hashes[i][j - 1] - hashes[i - 1][j - 1] + ((field[i][j] * deg_x) % p) * deg_y) % p;
      deg_x = (deg_x * x) % p;
    }
    deg_y = (deg_y * y) % p;
  }
  return hashes;
}

int64_t GetSubMatrixHash(std::vector<std::vector<int64_t>>& hashes, int64_t y_1, int64_t x_1, int64_t y_2, int64_t x_2) {
  int64_t answer = hashes[y_2][x_2];
  if (x_1 != 0) {
    answer -= hashes[y_2][x_1 - 1];
  }
  if (y_1 != 0) {
    answer -= hashes[y_1 - 1][x_2];
  }
  if (x_1 != 0 && y_1 != 0) {
    answer += hashes[x_1 - 1][y_1 - 1];
  }
  return answer;
}

bool SameHash(std::vector<std::vector<int64_t>>& hashes, int64_t y_1, int64_t x_1, int64_t y_2, int64_t x_2,
              std::vector<std::vector<int64_t>>& hashes_2, int64_t second_y_1, int64_t second_x_1, int64_t second_y_2, int64_t second_x_2) {
  return (((GetSubMatrixHash(hashes, y_1, x_1, y_2, x_2) * (static_cast<int64_t>(pow(x, second_x_1)) % p)) % p * (static_cast<int64_t>(pow(y, second_y_1)) % p)) % p == ((GetSubMatrixHash(hashes_2, second_y_1, second_x_1, second_y_2, second_x_2) * (static_cast<int64_t>(pow(x, x_1)) % p)) % p * (static_cast<int64_t>(pow(y, y_1)) % p)) % p);
}

bool IsSame(std::vector<std::vector<int64_t>>& hashes, int64_t y_1, int64_t x_1, int64_t y_2, int64_t x_2,
            int64_t second_y_1, int64_t second_x_1, int64_t second_y_2, int64_t second_x_2, std::vector<std::vector<int64_t>>& hashes_2) {
  int64_t left = y_1;
  int64_t right = y_2;
  int64_t middle = 0;
  int64_t max_pref = 0;
  int64_t row = 0;
  while (left < right) {
    middle = (left + right) / 2;
    if (SameHash(hashes, y_1, x_1, middle, x_2, hashes_2, second_y_1, second_x_1, middle - y_1, second_x_2)) {
      left = middle + 1;
      max_pref = middle - y_1 + 1;
      row = left;
    } else {
      right = middle;
    }
  }
  if (max_pref == y_2 - y_1 + 1) {
    return true;
  }
  left = y_1;
  right = y_2;
  middle = 0;
  int64_t max_suf = 0;
  while (left < right) {
    middle = (left + right + 1) / 2;
    if (SameHash(hashes, middle, x_1, y_2, x_2,hashes_2, middle - y_1, second_x_1, second_y_2, second_x_2)) {
      right = middle - 1;
      max_suf = y_2 - middle + 1;
    } else {
      left = middle;
    }
  }
  if (max_pref + max_suf < y_2 - y_1) {
    return false;
  }
  left = x_1;
  right = x_2;
  middle = 0;
  while (left < right) {
    middle = (left + right) / 2;
    if (SameHash(hashes, row, middle, row, x_2,hashes_2, second_y_1 - y_1 + row, middle - x_1, second_y_1 - y_1 + row, second_x_2)) {
      left = middle + 1;
      max_pref = middle - x_1 + 1;
    } else {
      right = middle;
    }
  }
  left = x_1;
  right = x_2;
  middle = 0;
  while (left < right) {
    middle = (left + right + 1) / 2;
    if (SameHash(hashes, row, x_1, row, middle,hashes_2, second_y_1 - y_1 + row, second_x_1, second_y_1 - y_1 + row, second_x_2 - x_2 + middle)) {
      right = middle - 1;
      max_suf = x_2 - middle + 1;
    } else {
      left = middle;
    }
  }
  if (max_pref + max_suf < x_2 - x_1) {
    return false;
  }
  return true;
}

int main() {
  int64_t field_height;
  int64_t field_width;
  std::cin >> field_height >> field_width;
  std::vector<std::vector<char>> field(field_height, std::vector<char>(field_width));
  for (int i = 0; i < field_height; ++i) {
    for (int j = 0; j < field_width; ++j) {
      std::cin >> field[i][j];
    }
  }
  std::vector<std::vector<int64_t>> field_hashes = PolynomialHash(field);
  int64_t map_height;
  int64_t map_width;
  std::cin >> map_height >> map_width;
  std::vector<std::vector<char>> map(map_height, std::vector<char>(map_width));
  for (int i = 0; i < map_height; ++i) {
    for (int j = 0; j < map_width; ++j) {
      std::cin >> map[i][j];
    }
  }
  std::vector<std::vector<int64_t>> map_hashes = PolynomialHash(map);
  //std::cout << (SameHash(field_hashes, 1, 0, 1, 1, 0, 0, 0, 1, map_hashes));
  //std::cout << (IsSame(field_hashes, 0, 0, 2, 1, 0, 0, map_height - 1, map_width - 1, map_hashes));
  //std::cout << (IsSame(field_hashes, 1, 0, 3, 1, 0, 0, map_height - 1, map_width - 1, map_hashes));
  //std::cout << (IsSame(field_hashes, 2, 0, 4, 1, 0, 0, map_height - 1, map_width - 1, map_hashes));
  std::cout << (IsSame(field_hashes, 2, 3, 4, 4, 0, 0, map_height - 1, map_width - 1, map_hashes));
  int64_t cnt = 0;
  for (int i = 0; i <= field_height - map_height; ++i) {
    for (int j = 0; j <= field_width - map_width; ++j) {
      if (IsSame(field_hashes, i, j, i + map_height - 1, j + map_width - 1, 0, 0, map_height - 1, map_width - 1, map_hashes)) {
        ++cnt;
      }
    }
  }
  std::cout << cnt;
}