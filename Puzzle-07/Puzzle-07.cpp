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

struct File
{
	unsigned long size;
};

class Folder
{
private:
	std::unordered_map<std::string, File> m_files;
	std::unordered_map<std::string, Folder> m_childs;
	Folder* m_parent;
	unsigned long m_size;

	void increment_size(unsigned long add)
	{
		m_size += add;
		if (m_parent) m_parent->increment_size(add);
	}

public:
	Folder(Folder* parent = nullptr)
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

	const unsigned size() const
	{
		return m_size;
	}

	void create_folder(const std::string &name)
	{
		Folder child(this);
		m_childs.insert({ name, child });
	}

	Folder* folder(const std::string& name)
	{
		return &m_childs.at(name);
	}

	const Folder* cfolder(const std::string& name) const
	{
		return &m_childs.at(name);
	}

	const std::vector<const Folder *> childs() const
	{
		std::vector<const Folder *> result;

		std::ranges::for_each(m_childs,
			[&result](const auto& f) {result.push_back(&(f.second)); });

		return result;
	}

	Folder* parent() const
	{
		return m_parent;
	}
};

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

Folder parse_input(std::istream& input)
{
	Folder root;

	Folder* current_path = &root;

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
					current_path = current_path->folder(command[2]);
			}
		}
		else if (command[0] == "dir")
		{
			current_path->create_folder(command[1]);
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

Folder parse_input(std::istream&& input)
{
	return parse_input(input);
}

void find_small_folders(const Folder& f, std::vector<unsigned long>& v)
{
	unsigned long sum = f.size();
	auto childs = f.childs();

	for (const auto& child : childs)
	{
		find_small_folders(*child, v);
	}

	if (sum < 100000) v.push_back(sum);
}

void part1(const Folder &root)
{
	std::vector<unsigned long> sizes;

	find_small_folders(root, sizes);
	if (root.size() < 100000) sizes.push_back(root.size());

	std::cout << "Total size of small folders: " << std::accumulate(sizes.cbegin(), sizes.cend(), 0ul) << std::endl;
}

void part2(const Folder& root)
{

}

int main()
{
	Timer t;

	Folder root = parse_input(std::ifstream("input.txt"));

	part1(root);
	part2(root);
}
