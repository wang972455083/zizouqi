

#include <memory.h>
#include <algorithm>
#include "algo_min_huns.h"


// 0��ʾ��������Ѿ��������ϵ���
#define TILE_BLOCK_MIN_HUNS__CHECK_AVAILABLE_TILES 0

// ����ֻ�ܳɿ��ӣ����Ժܼ�
static unsigned tile_block_min_huns__zipai(unsigned const p[9], unsigned head, unsigned tail) {
	unsigned min_hun = 0;
	for (unsigned i = head; i <= tail; ++i) {
		// 4��ͬ�������ƣ�������˳�ӣ�����һ�����ӣ���ʣһ�ţ�������Ҳ������ȥȡ���ֵ�����Կ϶�������
		if (TILE_BLOCK_MIN_HUNS__CHECK_AVAILABLE_TILES && p[i] == 4) return 999;
		min_hun += (3 - (p[i] % 3)) % 3;
	}
	return min_hun;
}

// �������ԣ��о���һЩ���ӣ����ֻ��ǵõݹ�ȥ�⣬ȡ��Сֵ
static unsigned tile_block_min_huns__shupai(unsigned p[9], unsigned head, unsigned tail, unsigned jiang_pos) {
	unsigned min_hun = 0;
	// ����ʼ�˿�ʼ��
	for (unsigned i = head; i <= tail; ++i) {
		// �Ȱ����λ���ϵ����еĿ�������
		// ���Է�֤Ϊʲô��ô����������Щ�����ܽ�ʡ�����ƣ������ǿ϶��Ǻͺ��������˳���ã����з����������
		//   ����1��˳�ӣ��鲻�鶼��Ҫһ�������ƣ�����Ļ��������������ƻ����ܸ����������˳����
		//   ����2��˳�ӣ���Ļ���������Ӿ�ȱ�ˣ�Ҳ�ò������ư�������Ļ�������Ļ����ܸ����������˳����
		//   ����3��˳�ӣ���Ҳ������3�����Ӱ���������Ҫ����������
		p[i] = p[i] % 3;
		// Ȼ�����p[i]����0��1��2���������
		if (0 == p[i]) continue; // ���λ�ÿ��ˣ�û����
		unsigned has1 = i + 1 <= tail ? p[i + 1] : 0;
		unsigned has2 = i + 2 <= tail ? p[i + 2] : 0;
		if (1 == p[i]) {
			// ��ʣһ���ƣ������ȥ��������˳�ӣ���Ȼ�ø�����������������
			// �˴�֮�䶼�ǽ�����ģ�������ĩ�˺���û���ˣ���Ȼ��ಹһ�������ƿ϶������˳��
			if (has1 && has2) {
				// ����˳�ӣ��Ǿ�������
				--p[i + 1];
				--p[i + 2];
			} else if (has1 || has2) {
				// ��һ�������ƾ�����˳����
				if (has1) --p[i + 1];
				else --p[i + 2];
				min_hun += 1;
			} else {
				// ��Ȼ���˷�������������
				p[i] = 0;
				min_hun += 2;
			}
		} else {
			// ��ʣ�����ƣ���1���������ܳɿ��ӣ�Ҳ���Ժͺ������2��˳�ӣ�Ҳ���Ժͺ������1��˳��
			// �����ֽ����ݹ��㣬ȡ��Сֵ
			// �ȿ�2��˳�����ַ�֧�ģ��л�����ǰ������
			if (has1 >= 2 && has2 >= 2) {
				unsigned tmp[9];
				memcpy(tmp, p, sizeof(tmp));
				tmp[i] = 0;
				tmp[i + 1] -= 2;
				tmp[i + 2] -= 2;
				unsigned needed_hun__shunzi2_branch = tile_block_min_huns__shupai(tmp, i + 1, tail, jiang_pos);
				if (needed_hun__shunzi2_branch <= 1) {
					min_hun += needed_hun__shunzi2_branch;
					return min_hun;
				}
				// �Լ���1�������Ƴɿ��Ӱ�
				p[i] = 0;
				unsigned needed_hun__kezi_brance;
				// �ڽ���λ�þͲ����������Ʋ�������
				if (TILE_BLOCK_MIN_HUNS__CHECK_AVAILABLE_TILES && i == jiang_pos) needed_hun__kezi_brance = 999;
				else needed_hun__kezi_brance = 1 + tile_block_min_huns__shupai(p, i + 1, tail, jiang_pos);
				min_hun += std::min(needed_hun__kezi_brance, needed_hun__shunzi2_branch);
				return min_hun;
			} else if (has1 >= 1 && has2 >= 1) {
				unsigned tmp[9];
				memcpy(tmp, p, sizeof(tmp));
				tmp[i] -= 1;
				tmp[i + 1] -= 1;
				tmp[i + 2] -= 1;
				unsigned needed_hun__shunzi1_branch = tile_block_min_huns__shupai(tmp, i, tail, jiang_pos);
				if (needed_hun__shunzi1_branch <= 1) {
					min_hun += needed_hun__shunzi1_branch;
					return min_hun;
				}
				// �Լ���1�������Ƴɿ��Ӱ�
				p[i] = 0;
				unsigned needed_hun__kezi_brance;
				// �ڽ���λ�þͲ����������Ʋ�������
				if (TILE_BLOCK_MIN_HUNS__CHECK_AVAILABLE_TILES && i == jiang_pos) needed_hun__kezi_brance = 999;
				else needed_hun__kezi_brance = 1 + tile_block_min_huns__shupai(p, i + 1, tail, jiang_pos);
				min_hun += std::min(needed_hun__kezi_brance, needed_hun__shunzi1_branch);
				return min_hun;
			} else {
				min_hun += 1;
				// �൱��β�ݹ�
			}
		}
	}
	return min_hun;
}



// ����Ԫ�ر�ʾ���λ�����м����ظ����ƣ�����1��9��9��λ��
unsigned malgo_min_huns(unsigned const p[9], bool is_zipai, unsigned jiang_pos) {
	// ���ƾͲ��÷ֿ�ȥ����
	if (is_zipai) return tile_block_min_huns__zipai(p, 0, 8);
	// ���ư�����ֿ���Ϊ�˽��ͷ�֧��ϣ��ɳ˷���ϵ��ɼӷ���ϵ
	unsigned min_hun = 0;
	// ��ֳ�block��block֮������Ҫ��2����λ
	for (unsigned i = 0; i <= 8; ) {
		if (0 == p[i]) {
			++i;
		} else {
			unsigned j = i;
			unsigned block[9] = { 0 };
			// �ҵ�block��ĩβ [i, j]
			for (;;) {
				block[j] = p[j];
				if (8 == j) break;
				if (0 == p[j + 1]) {
					if (7 == j) break;
					if (0 == p[j + 2]) break;
				}
				++j;
			}
			min_hun += tile_block_min_huns__shupai(block, i, j, jiang_pos);
			i = j + 3;
		}
	}
	return min_hun;
}

