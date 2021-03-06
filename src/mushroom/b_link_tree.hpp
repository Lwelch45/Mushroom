/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Mushroom
 *    > Created Time:  2016-10-07 20:10:30
**/

#ifndef _B_LINK_TREE_HPP_
#define _B_LINK_TREE_HPP_

#include "../include/utility.hpp"
#include "../include/atomic.hpp"

namespace Mushroom {

class KeySlice;
class Page;
class PoolManager;
class Latch;
class LatchManager;

class BLinkTree : private NoCopy
{
	public:
		class Iterator
		{
			public:
				Iterator(BLinkTree *b_link_tree, int32_t level = 0);

				~Iterator();

				bool Next();

				KeySlice *key_;

			private:
				BLinkTree *b_link_tree_;
				Page      *curr_;
				int32_t    level_;
				uint16_t   index_;
		};

		static const uint32_t MAX_KEY_LENGTH = 255;

		BLinkTree(uint32_t key_len);

		~BLinkTree();

		uint32_t KeyLength();

		bool Free();

		bool Put(KeySlice *key);

		bool Get(KeySlice *key);

	private:
		void Initialize();

		bool First(Page **page, int32_t level);

		bool Next(KeySlice *key, Page **page, uint16_t *index);

		struct Set {
			Set():depth_(0) { }
			page_t   page_no_;
			Latch   *latch_;
			Page    *page_;
			page_t   stack_[8];
			uint32_t depth_;
		};

		void DescendToLeaf(const KeySlice *key, Set &set);

		void SplitRoot(Set &set);

		void Insert(Set &set, KeySlice *key);

		LatchManager *latch_manager_;
		PoolManager  *pool_manager_;

		Atomic<page_t>  root_;

		uint8_t      key_len_;
		uint16_t     degree_;
};

} // namespace Mushroom

#endif /* _B_LINK_TREE_HPP_ */