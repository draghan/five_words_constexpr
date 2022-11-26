#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <iomanip>
#include <chrono>
using namespace std;

long long get_nanoseconds_since_epoch()
{
    return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}

long long timestamp_start = get_nanoseconds_since_epoch();

constexpr size_t WORD_LENGTH = 5;

consteval size_t get_next_word_end(const char* const text, size_t starting_index)
{
    while(text[starting_index] != '\0' && text[starting_index] != '\n')
    {
        ++starting_index;
    }
    return starting_index;
}

constexpr char* WORDS_ALPHA_CONTENT_STRING =
#include "words_alpha.generated.inc"
;

consteval size_t get_all_5_letter_words_count()
{
    size_t current_position_in_string = 0;
    size_t count_of_5_letter_words = 0;
    size_t next_word_end_position = get_next_word_end(WORDS_ALPHA_CONTENT_STRING, current_position_in_string);
    while(true)
    {
        if (next_word_end_position - current_position_in_string == WORD_LENGTH)
        {
            ++count_of_5_letter_words;
        }
        current_position_in_string = next_word_end_position + 1;
        if (WORDS_ALPHA_CONTENT_STRING[current_position_in_string] == '\0')
        {
            break;
        }
        next_word_end_position = get_next_word_end(WORDS_ALPHA_CONTENT_STRING, current_position_in_string);
    }
    return count_of_5_letter_words;
}

constexpr size_t ALL_5_LETTER_WORDS_COUNT = get_all_5_letter_words_count();

struct single_word_t
{
    char letters[WORD_LENGTH];

    [[nodiscard]] constexpr size_t size() const
    {
        return WORD_LENGTH;
    }
};

template<size_t N>
struct five_letter_words_array_t
{
    single_word_t words[N];

    [[nodiscard]] constexpr size_t size() const
    {
        return N;
    }
};

consteval five_letter_words_array_t<ALL_5_LETTER_WORDS_COUNT> get_all_five_letter_words()
{
    five_letter_words_array_t<ALL_5_LETTER_WORDS_COUNT> words_to_return{};

    size_t current_position_in_string = 0;
    size_t next_word_end_position = get_next_word_end(WORDS_ALPHA_CONTENT_STRING, current_position_in_string);
    size_t current_word_index = 0;
    while(true)
    {
        if (next_word_end_position - current_position_in_string == WORD_LENGTH)
        {
            words_to_return.words[current_word_index].letters[0] = WORDS_ALPHA_CONTENT_STRING[current_position_in_string + 0];
            words_to_return.words[current_word_index].letters[1] = WORDS_ALPHA_CONTENT_STRING[current_position_in_string + 1];
            words_to_return.words[current_word_index].letters[2] = WORDS_ALPHA_CONTENT_STRING[current_position_in_string + 2];
            words_to_return.words[current_word_index].letters[3] = WORDS_ALPHA_CONTENT_STRING[current_position_in_string + 3];
            words_to_return.words[current_word_index].letters[4] = WORDS_ALPHA_CONTENT_STRING[current_position_in_string + 4];
            ++current_word_index;
        }
        current_position_in_string = next_word_end_position + 1;
        if (WORDS_ALPHA_CONTENT_STRING[current_position_in_string] == '\0')
        {
            break;
        }
        next_word_end_position = get_next_word_end(WORDS_ALPHA_CONTENT_STRING, current_position_in_string);
    }
    return words_to_return;
}

constexpr auto ALL_FIVE_LETTER_WORDS = get_all_five_letter_words();

consteval size_t get_good_words_count()
{
    bool seen[26*26*26*26*26] = {false};
    char tmp[WORD_LENGTH];
    size_t good_words_count = 0;
    for (int w = 0; w < ALL_FIVE_LETTER_WORDS.size(); ++w)
    {
        tmp[0] = ALL_FIVE_LETTER_WORDS.words[w].letters[0];
        tmp[1] = ALL_FIVE_LETTER_WORDS.words[w].letters[1];
        tmp[2] = ALL_FIVE_LETTER_WORDS.words[w].letters[2];
        tmp[3] = ALL_FIVE_LETTER_WORDS.words[w].letters[3];
        tmp[4] = ALL_FIVE_LETTER_WORDS.words[w].letters[4];
        std::sort(tmp, tmp + WORD_LENGTH);
        bool bad_word = false;
        for (int j = 0; j < WORD_LENGTH - 1; ++j)
        {
            if (tmp[j] == tmp[j + 1])
            {
                bad_word = true;
                break;
            }
        }
        if (bad_word)
        {
            continue;
        }
        int hash = 0;
        for (char letter : tmp)
        {
            hash = hash*26 + letter - 'a';
        }
        if (seen[hash])
        {
            continue;
        }
        seen[hash] = true;
        ++good_words_count;
    }
    return good_words_count;
}

constexpr size_t GOOD_WORDS_COUNT = get_good_words_count();

consteval five_letter_words_array_t<GOOD_WORDS_COUNT> get_good_words()
{
    five_letter_words_array_t<GOOD_WORDS_COUNT> good_words{};
    bool seen[26*26*26*26*26] = {false};
    char tmp[WORD_LENGTH];
    int i = 0;
    for (size_t word_index = 0; word_index < ALL_FIVE_LETTER_WORDS.size(); ++word_index)
    {
        tmp[0] = ALL_FIVE_LETTER_WORDS.words[word_index].letters[0];
        tmp[1] = ALL_FIVE_LETTER_WORDS.words[word_index].letters[1];
        tmp[2] = ALL_FIVE_LETTER_WORDS.words[word_index].letters[2];
        tmp[3] = ALL_FIVE_LETTER_WORDS.words[word_index].letters[3];
        tmp[4] = ALL_FIVE_LETTER_WORDS.words[word_index].letters[4];

        std::sort(tmp, tmp + WORD_LENGTH);
        bool bad_word = false;
        for (int j = 0; j < WORD_LENGTH - 1; ++j)
        {
            if (tmp[j] == tmp[j + 1])
            {
                bad_word = true;
                break;
            }
        }
        if (bad_word)
        {
            continue;
        }

        int hash = 0;
        for (char letter : tmp)
        {
            hash = hash*26 + letter - 'a';
        }
        if (seen[hash])
        {
            continue;
        }
        seen[hash] = true;
        good_words.words[i].letters[0] = ALL_FIVE_LETTER_WORDS.words[word_index].letters[0];
        good_words.words[i].letters[1] = ALL_FIVE_LETTER_WORDS.words[word_index].letters[1];
        good_words.words[i].letters[2] = ALL_FIVE_LETTER_WORDS.words[word_index].letters[2];
        good_words.words[i].letters[3] = ALL_FIVE_LETTER_WORDS.words[word_index].letters[3];
        good_words.words[i].letters[4] = ALL_FIVE_LETTER_WORDS.words[word_index].letters[4];
        ++i;
    }
    return good_words;
}

constexpr five_letter_words_array_t<GOOD_WORDS_COUNT> GOOD_WORDS = get_good_words();

struct masks_array_t
{
    int masks[GOOD_WORDS_COUNT];
    [[nodiscard]] constexpr size_t size() const
    {
        return GOOD_WORDS_COUNT;
    }
};

consteval masks_array_t generate_masks()
{
    masks_array_t masks{};
    for (size_t i = 0 ; i < GOOD_WORDS_COUNT; ++i)
    {
        int mask = 0;
        for (size_t j = 0; j < GOOD_WORDS.words[i].size(); ++j)
        {
            mask |= 1 << (GOOD_WORDS.words[i].letters[j] - 'a');
        }
        masks.masks[i] = mask;
    }
    return masks;
}

constexpr masks_array_t MASKS = generate_masks();

constexpr size_t CAN_CONSTRUCT_SIZE = 1 << 26;

struct can_construct_array_t
{
    bool at[CAN_CONSTRUCT_SIZE];
};

consteval can_construct_array_t generate_can_construct_0()
{
    can_construct_array_t can_construct_0{};
    for (size_t i = 0 ; i < GOOD_WORDS_COUNT; ++i)
    {
        can_construct_0.at[MASKS.masks[i]] = true;
    }
    return can_construct_0;
}

constexpr can_construct_array_t CAN_CONSTRUCT_0 = generate_can_construct_0();

consteval can_construct_array_t generate_can_construct_1()
{
    can_construct_array_t can_construct_1{};
    for (int mask = 0; mask < CAN_CONSTRUCT_SIZE; ++mask)
    {
        if (!CAN_CONSTRUCT_0.at[mask])
        {
            continue;
        }
        for (size_t i = 0; i < GOOD_WORDS_COUNT; ++i)
        {
            if ((MASKS.masks[i] & mask) == 0)
            {
                can_construct_1.at[MASKS.masks[i] | mask] = true;
            }
        }
    }
    return can_construct_1;
}
constexpr can_construct_array_t CAN_CONSTRUCT_1 = generate_can_construct_1();

consteval can_construct_array_t generate_can_construct_2()
{
    can_construct_array_t can_construct_2{};
    for (int mask = 0; mask < CAN_CONSTRUCT_SIZE; ++mask)
    {
        if (!CAN_CONSTRUCT_1.at[mask])
        {
            continue;
        }
        for (size_t i = 0; i < GOOD_WORDS_COUNT; ++i)
        {
            if ((MASKS.masks[i] & mask) == 0)
            {
                can_construct_2.at[MASKS.masks[i] | mask] = true;
            }
        }
    }
    return can_construct_2;
}
constexpr can_construct_array_t CAN_CONSTRUCT_2 = generate_can_construct_2();

consteval can_construct_array_t generate_can_construct_3()
{
    can_construct_array_t can_construct_3{};
    for (int mask = 0; mask < CAN_CONSTRUCT_SIZE; ++mask)
    {
        if (!CAN_CONSTRUCT_2.at[mask])
        {
            continue;
        }
        for (size_t i = 0; i < GOOD_WORDS_COUNT; ++i)
        {
            if ((MASKS.masks[i] & mask) == 0)
            {
                can_construct_3.at[MASKS.masks[i] | mask] = true;
            }
        }
    }
    return can_construct_3;
}
constexpr can_construct_array_t CAN_CONSTRUCT_3 = generate_can_construct_3();

consteval can_construct_array_t generate_can_construct_4()
{
    can_construct_array_t can_construct_4{};
    for (int mask = 0; mask < CAN_CONSTRUCT_SIZE; ++mask)
    {
        if (!CAN_CONSTRUCT_3.at[mask])
        {
            continue;
        }
        for (size_t i = 0; i < GOOD_WORDS_COUNT; ++i)
        {
            if ((MASKS.masks[i] & mask) == 0)
            {
                can_construct_4.at[MASKS.masks[i] | mask] = true;
            }
        }
    }
    return can_construct_4;
}
constexpr can_construct_array_t CAN_CONSTRUCT_4 = generate_can_construct_4();

struct all_can_construct_arrays_t
{
    can_construct_array_t get[5];
};

constexpr all_can_construct_arrays_t CAN_CONSTRUCT_ARRAYS
{
    CAN_CONSTRUCT_0,
    CAN_CONSTRUCT_1,
    CAN_CONSTRUCT_2,
    CAN_CONSTRUCT_3,
    CAN_CONSTRUCT_4
};

consteval void calculate_all_sets_count(const all_can_construct_arrays_t &can_construct,
                                        vector<int> &result,
                                        int mask,
                                        int start_from,
                                        int& counter)
{
    if (result.size() == 5)
    {
        for (int i = 0; i < 5; ++i)
        {
            ++counter;
        }
        return;
    }
    for (int cur_word = start_from; cur_word < GOOD_WORDS_COUNT; ++cur_word)
    {
        if (((mask & MASKS.masks[cur_word]) == MASKS.masks[cur_word]) && (result.size() == 4 || can_construct.get[3 - result.size()].at[mask ^ MASKS.masks[cur_word]]))
        {
            result.push_back(cur_word);
            calculate_all_sets_count(can_construct, result, mask ^ MASKS.masks[cur_word], cur_word + 1, counter);
            result.pop_back();
        }
    }
}

consteval int calculate_results_count()
{
    vector<int> result;
    int result_counter = 0;
    for (int mask = 0; mask < CAN_CONSTRUCT_SIZE; ++mask)
    {
        if (CAN_CONSTRUCT_ARRAYS.get[4].at[mask])
        {
            calculate_all_sets_count(CAN_CONSTRUCT_ARRAYS, result, mask, 0, result_counter);
        }
    }
    return result_counter;
}

constexpr int RESULTS_COUNT = calculate_results_count();

consteval void calculate_all_sets(const all_can_construct_arrays_t &can_construct,
                                  vector<int> &result,
                                  int mask,
                                  int start_from,
                                  int & counter,
                                  five_letter_words_array_t<RESULTS_COUNT>& result_words)
{
    if (result.size() == 5)
    {
        for (int i = 0; i < 5; ++i)
        {
            result_words.words[counter].letters[0] = GOOD_WORDS.words[result[i]].letters[0];
            result_words.words[counter].letters[1] = GOOD_WORDS.words[result[i]].letters[1];
            result_words.words[counter].letters[2] = GOOD_WORDS.words[result[i]].letters[2];
            result_words.words[counter].letters[3] = GOOD_WORDS.words[result[i]].letters[3];
            result_words.words[counter].letters[4] = GOOD_WORDS.words[result[i]].letters[4];
            ++counter;
        }
        return;
    }
    for (int cur_word = start_from; cur_word < GOOD_WORDS_COUNT; ++cur_word)
    {
        if (((mask & MASKS.masks[cur_word]) == MASKS.masks[cur_word]) && (result.size() == 4 || can_construct.get[3 - result.size()].at[mask ^ MASKS.masks[cur_word]]))
        {
            result.push_back(cur_word);
            calculate_all_sets(can_construct, result, mask ^ MASKS.masks[cur_word], cur_word + 1, counter, result_words);
            result.pop_back();
        }
    }
}

consteval five_letter_words_array_t<RESULTS_COUNT> calculate_results()
{
    vector<int> result_tmp;
    five_letter_words_array_t<RESULTS_COUNT> result_words{};

    int counter = 0;
    for (int mask = 0; mask < CAN_CONSTRUCT_SIZE; ++mask)
    {
        if (CAN_CONSTRUCT_ARRAYS.get[4].at[mask])
        {
            calculate_all_sets(CAN_CONSTRUCT_ARRAYS, result_tmp, mask, 0, counter, result_words);
        }
    }
    return result_words;
}

constexpr five_letter_words_array_t<RESULTS_COUNT> RESULT_WORDS = calculate_results();

int main()
{
    auto file = fopen("solutions.txt", "w");
    int linebreak = 0;
    for(size_t current_word_index = 0; current_word_index < RESULT_WORDS.size(); ++current_word_index)
    {
        for(size_t current_word_letter = 0; current_word_letter < WORD_LENGTH; ++current_word_letter)
        {
            fputc(RESULT_WORDS.words[current_word_index].letters[current_word_letter], file);
        }
        fputc(' ', file);
        ++linebreak;
        if (linebreak == 5)
        {
            fputc('\n', file);
            linebreak = 0;
        }
    }
    fclose(file);
    long long timestamp_end = get_nanoseconds_since_epoch();
    std::cout << "Total time:   " << timestamp_end - timestamp_start << "ns (" << (timestamp_end - timestamp_start) / 1000 << "us)\n";
}
