#include <cassert>
#include <set>
#include <numeric>

#include "NORange.h"


long range::size() const
{
	return std::abs(end - start) + 1;
}

bool range::overlaps(const range& other) const
{
	return !(end < other.start || start > other.end);
}

void range::merge_with(const range& other)
{
	assert(this->overlaps(other));

	if (other.start < start) start = other.start;
	if (other.end > end) end = other.end;
}

bool range::operator < (const range& r) const
{
	return start < r.start;
}


void non_overlaping_ranges::insert(const range& r)
{
	if (ranges.empty()) ranges.insert(r);
	else
	{
		range merged_range = r;
		auto it = ranges.upper_bound(r);
		if (it != ranges.begin()) --it;

		while (it != ranges.end() && it->start <= r.end)
		{
			if (it->overlaps(merged_range))
			{
				merged_range.merge_with(*it);
				it = ranges.erase(it);
			}
			else ++it;
		}

		ranges.insert(merged_range);
	}
}

long non_overlaping_ranges::size() const
{
	return std::reduce(ranges.cbegin(), ranges.cend(), 0,
		[](const long acc, const range& r)
		{
			return acc + r.size();
		});
}

bool non_overlaping_ranges::is_inside(long x) const
{
	if (ranges.empty()) return false;
	auto it = ranges.upper_bound({ x,x });
	if (it == ranges.begin()) return false;
	--it;
	return x >= it->start && x <= it->end;
}

std::set<range>::iterator non_overlaping_ranges::begin()
{
	return ranges.begin();
}

std::set<range>::iterator non_overlaping_ranges::end()
{
	return ranges.end();
}
