#pragma once

using map = std::vector<std::string>;
using coord = std::pair<int, int>;

struct node
{
	double f{ std::numeric_limits<double>::infinity() };
	double g{ std::numeric_limits<double>::infinity() };
	coord parent;
};

std::vector<coord> aStar(const map& m, const coord& start,
	const std::function<bool(const coord&)> is_finish,
	const std::function<std::vector<coord>(const map&,const coord&)> get_neighbors,
	const std::function<double(const coord&)> heuristic);
