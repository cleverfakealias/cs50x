from cs50 import get_string


def count_letters(user_input):
    return len([letter for letter in user_input if letter.isalpha()])


def count_words(user_input):
    return len(user_input.split(" "))


def count_scentences(user_input):
    punctuaction_marks = [".", "!", "?"]
    return len([letter for letter in user_input if letter in punctuaction_marks])


def compute_coleman_laiu_index(letters, words, scentences):
    L = letters / words * 100
    S = scentences / words * 100
    return 0.0588 * L - 0.296 * S - 15.8


def print_recommended_grade_level(coleman_laiu_index):
    if coleman_laiu_index < 1:
        print("Before Grade 1")
    elif coleman_laiu_index >= 16:
        print("Grade 16+")
    else:
        print(f"Grade {round(coleman_laiu_index)}")


user_input = get_string("Text: ")
coleman_laiu_index = compute_coleman_laiu_index(
    count_letters(user_input),
    count_words(user_input),
    count_scentences(user_input))
print_recommended_grade_level(coleman_laiu_index)
