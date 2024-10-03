def get_valid_score(module_name):
    while True:
        try:
            score = float(input(f"Enter score for {module_name} (0-100): "))
            if 0 <= score <= 100:
                return score
            else:
                print("Error: Score must be between 0 and 100.")
        except ValueError:
            print("Error: Please enter a valid number.")

def calculate_average(scores):
    return sum(scores) / len(scores)

def get_classification(average):
    if average >= 69.5:
        return "1st (First-Class)"
    elif average >= 59.5:
        return "2.1 (Upper Second-Class)"
    elif average >= 49.5:
        return "2.2 (Lower Second-Class)"
    elif average >= 39.5:
        return "3rd"
    else:
        return "FAIL"

def main():
    modules = ["Module 1", "Module 2", "Module 3", "Module 4", "Module 5"]
    scores = []

    print("University of Suffolk Graduation Prize Determination Program")
    print("-----------------------------------------------------------")

    for module in modules:
        score = get_valid_score(module)
        scores.append(score)

    print("\nStudent's Scores:")
    for module, score in zip(modules, scores):
        print(f"{module}: {score}")

    average = calculate_average(scores)
    classification = get_classification(average)

    print(f"\nFinal Average: {average:.2f}")
    print(f"Classification: {classification}")

    if classification == "1st (First-Class)":
        print("Congratulations! The student qualifies for a graduation prize.")
    else:
        print("The student does not qualify for a graduation prize.")

if __name__ == "__main__":
    main()
