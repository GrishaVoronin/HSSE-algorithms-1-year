#include <iostream>
#include <vector>
#include <cmath>

long long MyLogUp(long long number) {
  long long add = 0;
  long long answer = 0;
  while (number > 1) {
    if (number % 2 == 1) {
      add = 1;
    }
    number /= 2;
    ++answer;
  }
  return answer + add;
}

long long MyLogDown(long long number) {
  long long answer = 0;
  while (number > 1) {
    number /= 2;
    ++answer;
  }
  return answer;
}

void GetEulerBypass(long long cur_vert, long long cur_depth, std::vector<std::vector<long long>>& adjacency_matrix, std::vector<long long>& depth, std::vector<long long>& euler_bypass) {
  depth[cur_vert] = cur_depth;
  euler_bypass.push_back(cur_vert);
  for (long long neighbour: adjacency_matrix[cur_vert]) {
    GetEulerBypass(neighbour, cur_depth + 1,adjacency_matrix, depth, euler_bypass);
    euler_bypass.push_back(cur_vert);
  }
}

std::vector<std::vector<std::pair<long long, long long>>> BuildSparseTable(std::vector<long long>& euler_bypass, std::vector<long long>& depth) {
  long long len = euler_bypass.size();
  std::vector<std::vector<std::pair<long long, long long>>> sparse_table(MyLogUp(len), std::vector<std::pair<long long, long long>>(len));
  for (long long i = 0; i < len; ++i) {
    sparse_table[0][i] = {depth[euler_bypass[i]], euler_bypass[i]};
  }
  for (long long i = 1; i < MyLogUp(len); ++i) {
    for (long long j = 0; j + pow(2, i) <= len; ++j) {
      sparse_table[i][j] = std::min(sparse_table[i - 1][j], sparse_table[i - 1][j + pow(2, i - 1)]);
    }
  }
  return sparse_table;
}

long long GetLCA(long long first, long long second, std::vector<std::vector<std::pair<long long, long long>>>& sparse_table, std::vector<long long>& first_entry) {
  long long left = first_entry[first];
  long long right = first_entry[second];
  if (left > right) {
    std::swap(left, right);
  }
  long long max_deg = MyLogDown(right - left + 1);
  long long answer = 0;
  if (sparse_table[max_deg][left].first < sparse_table[max_deg][right - pow(2, max_deg) + 1].first) {
    answer = sparse_table[max_deg][left].second;
  } else {
    answer = sparse_table[max_deg][right - pow(2, max_deg) + 1].second;
  }
  return answer;
}

int main() {
  long long vertexes_amount = 0;
  std::cin >> vertexes_amount;
  long long questions_amount = 0;
  std::cin >> questions_amount;
  std::vector<std::vector<long long>> adjacency_matrix(vertexes_amount);
  for (long long i = 1; i < vertexes_amount; ++i) {
    long long parent_i = 0;
    std::cin >> parent_i;
    adjacency_matrix[parent_i].push_back(i);
  }
  long long first = 0;
  long long second = 0;
  std::cin >> first >> second;
  long long x, y, z;
  std::cin >> x >> y >> z;
  if (vertexes_amount == 1) {
    std::cout << 0;
    return 0;
  }
  std::vector<long long> depth(vertexes_amount);
  std::vector<long long> euler_bypass;
  GetEulerBypass(0, 1, adjacency_matrix, depth, euler_bypass);
  std::vector<long long> first_entry(vertexes_amount, -1);
  for (size_t i = 0; i < euler_bypass.size(); ++i) {
    if (first_entry[euler_bypass[i]] == -1) {
      first_entry[euler_bypass[i]] = i;
    }
  }
  std::vector<std::vector<std::pair<long long, long long>>> sparse_table = BuildSparseTable(euler_bypass, depth);
  long long sum = 0;
  long long prev_answer = 0;
  for (long long i = 0; i < questions_amount; ++i) {
    sum += GetLCA((first + prev_answer) % vertexes_amount, second, sparse_table, first_entry);
    prev_answer = GetLCA((first + prev_answer) % vertexes_amount, second, sparse_table, first_entry);
    first = (x * first + y * second + z) % vertexes_amount;
    second = (x * second + y * first + z) % vertexes_amount;
  }
  std::cout << sum;
}