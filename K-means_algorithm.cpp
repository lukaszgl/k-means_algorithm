//This program is designed to assign datapoints to clases using K - means algorithm
//Autor: Łukasz Gębka

#include <iostream>
#include <vector>
#include <deque>
#include <stdexcept>
#include <cmath>
#include <string>
#include <fstream>
#include <iomanip>
#include <limits>
#include <Windows.h>
//bug fix
#undef max
//Define console color codes
#define COLOR_RESET   "\033[0m"
#define COLOR_BLACK   "\033[30m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_WHITE   "\033[37m"
#define COLOR_TABLE_TOP "\033[30;42m" //black font with green background
#define COLOR_TABLE_MAIN "\033[30;102m"  //black font with bright green background
#define PAUSE system("pause"); system("cls");

using namespace std;

//Declare global variables
int algorithm_iteration_count;
double max_error;
int number_of_classes;
int number_of_atributes;
double error;
int highest_number_of_digits;// Before the point

struct Point
{
	vector<double> atributes;
	//Returns false if size of atributes is different between two points
	bool checkSize(const Point& other)
	{
		if (this->atributes.size() != other.atributes.size())
		{
			return false;
		}
		else {
			return true;
		}
	}
	double EucllideanDistanceSquared(const Point& a)
	{
		if (!checkSize(a))
		{
			throw invalid_argument("Points have different atribute amounts");
		}
		double distance_squared = 0;
		for (int i = 0; i < a.atributes.size(); i++)
		{
			distance_squared += pow((atributes[i] - a.atributes[i]), 2);
		}
		return distance_squared;
	}
	Point operator=(const Point& other)
	{
		atributes.erase(atributes.begin(), atributes.end());
		for (int i = 0; i < other.atributes.size(); i++)
		{
			atributes.push_back(other.atributes[i]);
		}
		return *this;
	}
};
struct Centroid :public Point
{
	vector<Point*> assigned_points;

	void assignPoint(Point* a)
	{
		assigned_points.push_back(a);
	}
	void eraseAssignedPoints()
	{
		assigned_points.erase(assigned_points.begin(), assigned_points.end());
	}
	double getMeanDistance()
	{
		double sum = 0;
		for (int i = 0; i < assigned_points.size(); i++)
		{
			sum += sqrt(EucllideanDistanceSquared(*assigned_points[i]));
		}
		return sum / assigned_points.size();
	}
	double getInertia()
	{
		double inertia = 0;
		for (int i = 0; i < assigned_points.size(); i++)
		{
			inertia += sqrt(EucllideanDistanceSquared(*assigned_points[i]));
		}
		return inertia;
	}
	void relocateToMeanValues()
	{
		for (int i = 0; i < atributes.size(); i++)
		{
			double sum = 0;
			for (int j = 0; j < assigned_points.size(); j++)
			{
				sum += assigned_points[j]->atributes[i];
			}
			this->atributes[i] = sum / assigned_points.size();
		}
	}
	Centroid operator=(const Point& other)
	{
		atributes.erase(atributes.begin(), atributes.end());

		for (int i = 0; i < other.atributes.size(); i++)
		{
			atributes.push_back(other.atributes[i]);
		}
		return *this;
	}
};
int NumberOfDigits(double var)
{
	return to_string((int)var).size();
}
void Print(const string& text, ostream& stream = cout, bool cursor = false, string color = COLOR_RESET)
{
	if (&stream == &cout)
	{
		stream << color << text << endl;
		stream << COLOR_RESET;
		if (cursor)
		{
			stream << "-> ";
		}
	}
	else
	{
		stream << text << endl;
		if (cursor)
		{
			stream << "-> ";
		}
	}
	stream.flush();
}
void ExcPrint(exception& err, bool user_action = false)
{
	cout << COLOR_RED << err.what() << COLOR_RESET << endl;
	if (user_action)
	{
		cout << "Try again" << endl << "-> ";
	}
}
int getInt()
{
	int input = 0;
	while (true)
	{
		cin >> input;
		if (!cin)
		{
			if (cin.bad() || cin.eof())
			{
				throw invalid_argument("IRRECOVERABLE_INPUT_ERROR");
			}
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			Print("Enter an intiger");
		}
		else
		{
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			return input;
		}
	}
}
bool getBool()
{
	string input;
	bool boolean;
	getline(cin, input);
	input = toupper(input[0]);
	while (input != "Y" && input != "N")
	{
		Print("Invalid input, please enter Y or N", cout, 1);
		getline(cin, input);
		input = toupper(input[0]);
	}
	return input == "Y" ? boolean = true : boolean = false;
}
ofstream openOutputFile(const string& file_name)
{
	ofstream output_file(file_name);

	if (output_file)
	{
		return output_file;
	}
	else
		throw invalid_argument("CAN_NOT_CREATE_FILE");
}
ifstream openInputFile(const string& file_name)
{
	ifstream data_file(file_name);

	if (data_file)
	{
		return data_file;
	}
	else
		throw invalid_argument("CAN_NOT_OPEN_FILE");
}
ifstream getValidInputFile()
{
	ifstream data_file;
	string file_name;
	bool retry = false;
	do
	{
		try
		{
			getline(cin, file_name);
			data_file = openInputFile(file_name);
			retry = false;
		}
		catch (invalid_argument& err)
		{
			ExcPrint(err, true);
			retry = true;
		}
	} while (retry);
	return data_file;
}
ofstream getValidOutputFile()
{
	ofstream output_file;
	string file_name;
	bool retry = false;
	do
	{
		try
		{
			getline(cin, file_name);
			output_file = openOutputFile(file_name);
			retry = false;
		}
		catch (invalid_argument& err)
		{
			ExcPrint(err, true);
			retry = true;
		}
	} while (retry);
	return output_file;
}
void readInputFile(istream& stream, deque<Point>& data)
{
	//Get variables on top of the file
	if (!(stream >> algorithm_iteration_count >> max_error >> number_of_classes)) {
		throw invalid_argument("TOP_VARIABLE_VALUE");
	}
	//Number of classes can not equal 0
	if (number_of_classes == 0)
	{
		throw invalid_argument("TOP_CLASS_COUNT_IS_NULL");
	}
	//Clear file buffer
	string ignore_line;
	getline(stream, ignore_line);

	//Get atributes
	string line;
	while (!stream.eof())
	{
		getline(stream, line);
		Point data_point;
		string atribute;
		for (int i = 0; i < line.size(); i++)
		{
			if (line[i] != ' ' && i != line.size() - 1)
			{
				atribute += line[i];
			}
			else if (i == line.size() - 1)
			{
				atribute += line[i];
				try
				{
					stod(atribute);
				}
				catch (exception& exc)
				{
					throw invalid_argument("CLASS_ATRIBUTE_VALUE");
				}
				data_point.atributes.push_back(stod(atribute));

				//Get highest digit count before the point
				if (highest_number_of_digits < NumberOfDigits(data_point.atributes.back()))
				{
					highest_number_of_digits = NumberOfDigits(data_point.atributes.back());
				}
				atribute = "";
			}
			else
			{
				try
				{
					stod(atribute);
				}
				catch (exception& exc)
				{
					throw invalid_argument("CLASS_ATRIBUTE_VALUE");
				}
				data_point.atributes.push_back(stod(atribute));

				//Get highest digit count before the point
				if (highest_number_of_digits < NumberOfDigits(data_point.atributes.back()))
				{
					highest_number_of_digits = NumberOfDigits(data_point.atributes.back());
				}
				atribute = "";
			}
		}
		data.push_back(data_point);
	}

	//Check for any missing or extra point atributes
	number_of_atributes = data[0].atributes.size();
	for (int i = 1; i < data.size(); i++)
	{
		if (number_of_atributes != data[i].atributes.size())
		{
			throw invalid_argument("CLASS_ATRIBUTE_COUNT");
		}
	}

	//Number of classes can't be greater than number of points
	if (number_of_classes > data.size())
	{
		throw invalid_argument("TOP_CLASS_COUNT_TOO_BIG");
	}
	return;
}
double EucllideanDistanceSquared(const Point& a, const Point& b)
{
	if (a.atributes.size() != b.atributes.size())
	{
		throw invalid_argument("POINT_ATRIBUTE_AMOUNT");
	}
	double return_value = 0;
	for (int i = 0; i < a.atributes.size(); i++)
	{
		return_value += pow((a.atributes[i] - b.atributes[i]), 2);
	}
	return return_value;
}
void initializeRandomly(const deque<Point>& data, deque<Centroid>& centroids)
{
	//Assign centroids to random points, points don't repeat
	srand(time(NULL));
	vector<int> previous_numbers;
	bool repeated_number;
	int random_index;
	centroids.erase(centroids.begin(), centroids.end());
	for (int i = 0; i < number_of_classes; i++)
	{
		centroids.push_back(Centroid());
		do
		{
			repeated_number = false;
			random_index = rand() % data.size();
			for (int j = 0; j < previous_numbers.size(); j++)
			{
				if (previous_numbers[j] == random_index)
				{
					previous_numbers.erase(previous_numbers.begin() + j);
					repeated_number = true;
					break;
				}
			}
			previous_numbers.push_back(random_index);
		} while (repeated_number);
		centroids[i] = data[random_index];
	}
}
void iterateAlgorithm(deque<Point>& data, deque<Centroid>& centroids)
{
	//Clear assigned points if there are any
	for (int i = 0; i < number_of_classes; i++)
	{
		centroids[i].eraseAssignedPoints();
	}
	//Assign points to the closest centroid
	for (int i = 0; i < data.size(); i++)
	{
		double min_distance = EucllideanDistanceSquared(data[i], centroids[0]);
		int closest_centroid_index = 0;
		for (int j = 1; j < number_of_classes; j++)
		{
			if (EucllideanDistanceSquared(data[i], centroids[j]) < min_distance)
			{
				min_distance = EucllideanDistanceSquared(data[i], centroids[j]);
				closest_centroid_index = j;
			}
		}
		centroids[closest_centroid_index].assignPoint(&data[i]);
	}
	//Relocate centroids to their respective mean values
	for (int i = 0; i < number_of_classes; i++)
	{
		centroids[i].relocateToMeanValues();
	}
}
void runAlgorithm(deque<Point>& data, deque<Centroid>& centroids)
{
	//Initialize centroids
	initializeRandomly(data, centroids);
	//Perform iterations
	for (int i = 0; i < algorithm_iteration_count; i++)
	{
		try {
		iterateAlgorithm(data, centroids);
		}
		catch(exception exc){
			ExcPrint(exc);
			return;
		}
	}

	//Calculate error
	double inertia = 0;
	for (int i = 0; i < centroids.size(); i++)
	{
		inertia += centroids[i].getInertia();
	}
	error = inertia;
}
void displayResults(ostream& stream, int precision, const deque<Point>& data, const deque<Centroid>& centroids)
{
	//Clear screen
	system("cls");
	//Save default precision
	streamsize default_precision = stream.precision();
	//Set precision of data
	stream << setprecision(precision) << fixed;
	//Print error
	if (error > max_error)
	{
		Print("Max error exceeded with error = ", stream, 0, COLOR_YELLOW);
		stream << error << endl;
		Print("Consider increasing iteration count", stream, 0, COLOR_YELLOW);
	}
	else stream << "Error = " << error << endl;

	//Set color (black font with green background)
	stream << COLOR_TABLE_TOP;
	//Start printing top of the table
	stream << "||Centroid";
	//Compute number of characters in column
	int number_of_characters_in_column;
	if (precision == 0)
	{
		//2 is representing "-" " "
		number_of_characters_in_column = 2 * number_of_atributes + (highest_number_of_digits * number_of_atributes);
	}
	else
	{
		//precision+3 is representing "-" "." " " and the digits after point
		number_of_characters_in_column = (precision + 3) * number_of_atributes + (highest_number_of_digits * number_of_atributes);
	}


	//This and the following for loop add spaces for the table to be even
	for (int i = 0; i < number_of_characters_in_column - 8; i++)
	{
		stream << " ";
	}
	stream << "||Cluster id ";
	stream << "||Point";                                          
	for (int i = 0; i < number_of_characters_in_column - 5; i++)
	{
		stream << " ";
	}
	stream << endl;

	//Start printing contents of the table
	for (int i = 0; i < centroids.size(); i++)
	{
		for (int point_number = 0; point_number < centroids[i].assigned_points.size(); point_number++)
		{
			//Switch background color to bright green (it is in the loop to fix a bug)
			stream << COLOR_TABLE_MAIN;
			//Print centroid atributes
			stream << "||";
			for (int j = 0; j < number_of_atributes; j++)
			{
				stream << centroids[i].atributes[j] << " ";
			}
			//If atribute is non negative, add spaces to compensate for - sign and allign the table
			//Additionally add extra spaces if number of digits before point is lower than highest
			for (int j = 0; j < number_of_atributes; j++)
			{
				if (centroids[i].atributes[j] >= 0)
					stream << " ";
				if (NumberOfDigits(centroids[i].atributes[j]) < highest_number_of_digits)
				{
					for (int ind = 0; ind < (highest_number_of_digits - NumberOfDigits(centroids[i].atributes[j])); ind++)
					{
						stream << " ";
					}
				}
			}
			stream << "||";
			stream << i;
			//This loop adds spaces to allign the table
			for (int j = 0; j < 11 - to_string(i).size(); j++)//j<(size of Centroid id) - (number of digits of i(centroid index))
			{
				stream << " ";
			}
			stream << "||";
			//Print point atributes
			for (int j = 0; j < centroids[i].assigned_points[point_number]->atributes.size(); j++)
			{

				stream << centroids[i].assigned_points[point_number]->atributes[j] << " ";
			}
			//If atribute is non negative, add spaces to compensate for - sign and allign the table
			//Additionally add extra spaces if number of digits before point is lower than highest
			for (int j = 0; j < centroids[i].assigned_points[point_number]->atributes.size(); j++)
			{
				if (centroids[i].assigned_points[point_number]->atributes[j] >= 0)
					stream << " ";
				if (NumberOfDigits(centroids[i].assigned_points[point_number]->atributes[j]) < highest_number_of_digits)
				{
					for (int ind = 0; ind < (highest_number_of_digits - NumberOfDigits(centroids[i].assigned_points[point_number]->atributes[j])); ind++)
					{
						stream << " ";
					}
				}
			}
			//Reset color(bug fix)
			stream << COLOR_RESET;

			stream << endl;
		}
	}
	//Reset changed stream parameters
	stream << setprecision(default_precision);
	stream << COLOR_RESET;
}
void displayResultsRaw(ostream& stream, int precision,const deque<Point>& data,const deque<Centroid>& centroids)
{
	//Save default precision
	streamsize default_precision = stream.precision();

	//Set precision of data
	stream << setprecision(precision) << fixed;

	//Print error and current precision
	stream << "Precision = " << precision << " " << "Error = " << error << endl;

	//Print table
	stream << "Centroid Cluster_id Point" << endl;
	for (int i = 0; i < centroids.size(); i++)
	{
		for (int point_number = 0; point_number < centroids[i].assigned_points.size(); point_number++)
		{
			for (int j = 0; j < number_of_atributes; j++)
			{
				stream << centroids[i].atributes[j] << " ";
			}

			stream << i << " ";

			for (int j = 0; j < centroids[i].assigned_points[point_number]->atributes.size(); j++)
			{
				stream << centroids[i].assigned_points[point_number]->atributes[j] << " ";
			}
			stream << endl;
		}
	}
	stream << setprecision(default_precision);
}
void displayManual()
{
	system("cls");
	Print("1.Input file format", cout, 0, COLOR_BLUE);
	cout << "Variables stored in data file should be separated with spaces and put in following order:\n"
	"algorithm_iteration_count max_error number_of_classes\n"
	"point1_atribute_1 point1_atribute_2 ... point1_atribute_n\n"
	"point2_atribute_1 point2_atribute_2 ... point2_atribute_n\n"
	".\n"
	".\n"
	".\n"
	"pointm_atribute_1 pointm_atribute_2 ... pointm_atribute_n\n"
	"\n"
	"where:\n"
	"n is number of atributes(it has to be the same in every point)\n"
	"m is number of points\n"
	"\n";
	Print("2.Input file errors", cout, 0, COLOR_BLUE);
	cout<<"Data extractrion from file can return following errors:\n"
	COLOR_RED "CAN_NOT_OPEN_FILE"<< COLOR_RESET << "-> program is unable to open file, check file path and spelling\n"
	COLOR_RED "TOP_VARIABLE_VALUE"<< COLOR_RESET << "-> unable to convert the top theree variables\n"
	COLOR_RED "TOP_CLASS_COUNT_IS_NULL"<< COLOR_RESET << "-> number of classes is zero\n"
	COLOR_RED "TOP_CLASS_COUNT_TO_BIG"<< COLOR_RESET << "-> number of classes is bigger than number of points\n"
	COLOR_RED "CLASS_ATRIBUTE_VALUE"<< COLOR_RESET << "-> unable to convert class atribute\n"
	COLOR_RED "CLASS_ATRIBUTE_COUNT"<< COLOR_RESET << "-> point may have a missing or extra atribute\n"
	"\n";
	Print("3.Output file format", cout, 0, COLOR_BLUE);
	cout<<"Output file is written with minimal formating for easier exporting and is formated the following way:\n"
	"\n"
	"\"Precision =\" decimal_precision \"Error =\" error\n"
	"\"Centroid Cluster_id Point\"\n"
	"centroid1_atributes cluster1_id first_assigned_point_atributes\n"
	".\n"
	".\n"
	".\n"
	"centroid1_atributes cluster1_id last_assigned_point_atributes\n"
	"centroid2_atributes cluster2_id first_assigned_point_atributes\n"
	".\n"
	".\n"
	".\n"
	"centroid(number_of_classes)_atributes cluster(number_of_classes)_id last_assigned_point_atributes\n"
	"\n";
	Print("4.Output file errors", cout, 0, COLOR_BLUE);
	cout<< COLOR_RED "CAN_NOT_CREATE_FILE"<< COLOR_RESET "-> can not create file, try changing path or check disk health"<<endl;
}
void displayMenu(bool bad_variables)
{
	Print("This program is designed to assign datapoints to clases using K-means algorithm");
	Print("To avoid unexpected behaviour do not change window size when not in menu");
	Print(bad_variables ? COLOR_RED"Data is not valid, you need to extract it first" : COLOR_GREEN"Data is valid, algorithm is ready");
	Print("1. Extract data from file");
	Print("2. Run algorithm");
	Print("3. Help");
	Print("0. Exit the program", cout, 1);
}
void setupTerminal()
{
	//Enable ANSI sequences in terminal, required for colors
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;
	GetConsoleMode(hOut, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hOut, dwMode);
	//Set color to default (bug fix)
	cout << COLOR_RESET;
}
int main()
{
	deque<Point>* data = new deque<Point>;
	deque<Centroid>* centroids = new deque<Centroid>;

	bool bad_variables = true;
	//Enable ANSI sequences in terminal, required for colors
	setupTerminal();

	while (true)
	{	//Catch any unhandled exceptions and safely exit program
		try {
			displayMenu(bad_variables);
			int choice = 0;
			choice = getInt();
			switch (choice)
			{
			case 1:
			{
				//Check if variables have already been initialized
				if (!bad_variables)
				{
					bool overwrite;
					Print("Variables have already been initialized, do you wish to overwrite the data? (Y/N)", cout, 1);
					overwrite = getBool();
					if (overwrite)
					{
						data->erase(data->begin(), data->end());
						centroids->erase(centroids->begin(), centroids->end());
					}
					else {
						system("cls");
						break;
					}
				}

				//Get valid file, handle exceptions
				Print("Enter file path, remember to add the file extension to the name (egz .txt)", cout, 1);
				ifstream data_file;
				data_file = getValidInputFile();

				//Read from file, handle exceprions
				try
				{
					readInputFile(data_file, *data);
					bad_variables = false;
				}
				catch (invalid_argument& err)
				{
					ExcPrint(err);
				}
				data_file.close();
				PAUSE
				break;
			}
			case 2:
			{
				//Check variables
				if (bad_variables)
				{
					Print(COLOR_RED"No data, you have to extract it first");
					PAUSE
					break;
				}

				//Run algorithm
				runAlgorithm(*data, *centroids);

				//Display data
				Print("Set decimal precision of data to be displayed");
				Print("Low precision can cause table to be unaligned in specific cases", cout, 1);
				int precision = getInt();
				displayResults(cout, precision, *data, *centroids);

				//Save file if needed
				Print("Do you wish to save raw data to file? (Y/N)");
				bool save;
				save = getBool();
				switch (save)
				{
				case true:
				{
					Print("Enter file path, remember to add the file extension to the name (egz .txt)", cout, 1);
					ofstream output_file;
					output_file = getValidOutputFile();
					displayResultsRaw(output_file, precision, *data, *centroids);
					output_file.close();
					break;
				}
				default:
				{
					break;
				}
				}
				PAUSE;
				break;
			}
			case 3:
			{
				displayManual();
				PAUSE
				break;
			}
			case 0:
			{
				delete data;
				delete centroids;
				return 0;
			}
			default:
			{
				system("cls");
				break;
			}
			}
		}
		catch (exception& err)
		{
			ExcPrint(err);
			Print("Irrecoverable error occured, closing the program", cout, false, COLOR_RED);
			delete data;
			delete centroids;
			return -1;
		}
	}
	delete data;
	delete centroids;
	return 0;
}
