#pragma once

struct range
{
	long start = 0;
	long end = 0;

	long size() const;
	bool overlaps(const range& other) const;
	void merge_with(const range& other);
	bool operator < (const range& r) const;
};

class non_overlaping_ranges
{
private:
	std::set<range> ranges;

public:
	void insert(const range& r);
	long size() const;
	bool is_inside(long x) const;
	std::set<range>::iterator begin();
	std::set<range>::iterator end();
};
