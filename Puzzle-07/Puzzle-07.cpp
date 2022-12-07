#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <vector>
#include <string>
#include <ranges>
#include <algorithm>
#include <numeric>

#include "../utils/timer.h"

std::vector<std::string> parse_command(const std::string& str)
{
	std::vector<std::string> result;

	std::ranges::for_each(str | std::views::split(' '),
		[&result](const auto& v)
		{
			result.push_back(std::string(v.begin(), v.end()));
		}
	);

	return result;
}

struct File
{
	unsigned long size;
};

class Dir
{
private:
	std::unordered_map<std::string, File> m_files;
	std::unordered_map<std::string, Dir> m_childs;
	Dir* m_parent;
	unsigned long m_size;

	void increment_size(unsigned long add)
	{
		m_size += add;
		if (m_parent) m_parent->increment_size(add);
	}

public:
	Dir(Dir* parent = nullptr)
		:m_parent(parent), m_size(0)
	{
	}

	void create_file(const std::string &name, unsigned long size)
	{
		m_files[name].size = size;
		m_size += size;
		if (m_parent) m_parent->increment_size(size);
	}

	const File& file(const std::string& name) const
	{
		return m_files.at(name);
	}

	const unsigned long size() const
	{
		return m_size;
	}

	void create_dir(const std::string &name)
	{
		Dir child(this);
		m_childs.insert({ name, child });
	}

	Dir* dir(const std::string& name)
	{
		return &m_childs.at(name);
	}

	const Dir* cdir(const std::string& name) const
	{
		return &m_childs.at(name);
	}

	const std::vector<const Dir *> childs() const
	{
		std::vector<const Dir *> result;

		std::ranges::for_each(m_childs,
			[&result](const auto& f) {result.push_back(&(f.second)); });

		return result;
	}

	Dir* parent() const
	{
		return m_parent;
	}

	bool operator < (const Dir& dir) const
	{
		return m_size < dir.size();
	}
};

Dir parse_input(std::istream& input)
{
	Dir root;

	Dir* current_path = &root;

	std::string line;
	while (std::getline(input, line))
	{
		auto command = parse_command(line);

		if (command[0] == "$")
		{
			if (command[1] == "cd")
			{
				if (command[2] == "/")
					current_path = &root;
				else if (command[2] == "..")
					current_path = current_path->parent();
				else
					current_path = current_path->dir(command[2]);
			}
		}
		else if (command[0] == "dir")
		{
			current_path->create_dir(command[1]);
		}
		else
		{
			try
			{
				current_path->create_file(command[1], std::stoul(command[0]));
			}
			catch(...) {}
		}
	}

	return root;
}

constexpr unsigned long disk_size = 70000000;
constexpr unsigned long needed_free_space = 30000000;

unsigned long find_bigger_dir_smaller_than(const Dir &dir, unsigned long need_to_free)
{
	unsigned long smaller = disk_size;

	for (const auto& child : dir.childs())
	{
		auto size = find_bigger_dir_smaller_than(*child, need_to_free);
		if (size >= need_to_free)
			smaller = std::min(smaller, size);
	}

	if (dir.size() >= need_to_free)
		smaller = std::min(smaller, dir.size());

	return smaller;
}

void part2(const Dir& root)
{
	const unsigned long need_to_free = root.size() - (disk_size - needed_free_space);

	unsigned int r = find_bigger_dir_smaller_than(root, need_to_free);

	std::cout << "          Total file size: " << root.size() << std::endl;
	std::cout << "             Need to free: " << need_to_free << std::endl;
	std::cout << "Smallest folder to delete: " << r << std::endl;
}


Dir parse_input(std::istream&& input)
{
	return parse_input(input);
}

void find_small_folders(const Dir& f, std::vector<unsigned long>& v)
{
	unsigned long sum = f.size();
	auto childs = f.childs();

	for (const auto& child : childs)
	{
		find_small_folders(*child, v);
	}

	if (sum < 100000) v.push_back(sum);
}

void part1(const Dir &root)
{
	std::vector<unsigned long> sizes;

	find_small_folders(root, sizes);
	if (root.size() < 100000) sizes.push_back(root.size());

	std::cout << "Total size of small folders: " << std::accumulate(sizes.cbegin(), sizes.cend(), 0ul) << std::endl;
}

int main()
{
	Timer t;

	Dir root = parse_input(std::ifstream("input.txt"));

	part1(root);
	part2(root);
}
