

void read_database(unordered_map<string, double> &database)
{
	ifstream file("database.txt");
	string element;
	double atomic_mass;
	while (file >> element >> atomic_mass) {
		database[element] = atomic_mass;
	}
	file.close();
}

bool cheking_balance(string &formula)
{
	bool result = true;
	int balance = 0;
	for (int i = 0; i < (int)formula.size(); i++) {
		if (formula[i] == ')') balance--;
		if (formula[i] == '(') balance++;
		if (balance < 0) result = false;
	}
	if (balance != 0) result = false;
	return result;
}

string read_element(string &formula, int &position)
{
	string element = "";
	element += formula[position];
	position++;
	while ('a' <= formula[position] && formula[position] <= 'z') {
		element += formula[position];
		position++;
	}
	return element;
}

int read_number(string &formula, int &position)
{
	int result = 0;
	while ('1' <= formula[position] && formula[position] <= '9'){
		result = result * 10 + (formula[position] - '0');
		position++;
	}
	if (result == 0) result = 1;
	return result;
}

unordered_map<string, int> calculation(string &formula, int &position, bool &correctly)
{
	unordered_map<string, int> number_atoms;
	bool end = false;
	while (!end && position < (int)formula.size() - 1) {
		if ('A' <= formula[position] && formula[position] <= 'Z') {
			string current_element = read_element(formula, position);
			int number = read_number(formula, position);
			number_atoms[current_element] += number;
		}

		if (formula[position] == ')') {
			position++;
			end = true;
		}

		if (formula[position] == '(') {
			position++;
			unordered_map<string, int> number_atoms_subformula;
			number_atoms_subformula = calculation(formula, position, correctly);
			int number = read_number(formula, position);
			for (auto atom : number_atoms_subformula) {
				number_atoms[atom.first] += atom.second * number;
			}
		}
	}
	if (number_atoms.size() == 0) correctly = false;
	return number_atoms;
}

bool cheking_elements(unordered_map<string, double> database, unordered_map<string, int> numbers_atoms)
{
	bool result = true;
	for (auto atom : numbers_atoms) {
		if (database.find(atom.first) == database.end()) {
			result = false;
		}
	}
	return result;
}

void output(unordered_map<string, double> &database, unordered_map<string, int> &numbers_atoms)
{
	cout << "Элемент   Атомная масса   Атомы   Результат" << endl;
	double total = 0;
	for (auto atom : numbers_atoms) {

		cout << setw(7) << right << atom.first;
		cout << fixed << setw(16) << right << setprecision(2) << database[atom.first];
		cout << fixed << setw(8) << right << atom.second;
		double value = atom.second * database[atom.first];
		cout << fixed << setw(12) << right << setprecision(2) << value << endl;
		total += value;
	}
	cout << fixed << setw(43) << right << setprecision(2) << total << endl;
	system("pause");
}

void output_error(int error)
{
	switch (error)
	{
	case 1: cout << "Ошибка в расставление скобок!" << endl; break;
	case 2: cout << "Введён не существующий элемент!" << endl; break;
	case 3: cout << "Неверная формула" << endl; break;
	}
	system("pause");
}

int main()
{
	setlocale(LC_ALL, "");

	unordered_map<string, double> database;
	read_database(database);

	string formula;
	cin >> formula;

	bool correctly = cheking_balance(formula);
	if (correctly) {
		formula += '#';
		int position = 0;
		unordered_map<string, int> numbers_atoms;
		numbers_atoms = calculation(formula, position, correctly);

		if (correctly) {
			correctly = cheking_elements(database, numbers_atoms);
			if (correctly) output(database, numbers_atoms);
			else output_error(2);
		}
		else output_error(3);
	}
	else output_error(1);

	return 0;
}