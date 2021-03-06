/**
 * @file network.cpp
 * @author Daniel Felipe
 * @date 03/01/2021
 *
 * @brief This is the source file of all functions used in this code.
 */


#include <network.h>

#include <iostream>
#include <fstream>
#include <algorithm>


void read_network(std::vector<std::vector<std::string> > &nodes, std::vector<std::pair<std::string, std::string> > &links){
  // File to be read
  std::string line;

  // Check the file was opened
  std::ifstream file(MyPar.filename);
  if(!file.is_open()){
    std::cerr << "Could not open the file '" << MyPar.filename << "'" << std::endl;
    return;
  }

  // Variable to save the data
  std::vector<std::vector<std::string> > data;

  // Loop over lines 'till file end
  while(std::getline(file, line)){
    // Transform line to Uppercase
    std::transform(line.begin(), line.end(), line.begin(), ::toupper);

    // Check for the separated by a delimiter
    std::vector<std::string> newline;
    std::string::size_type found = line.find(",");

    // Loop 'till the last delimiter
    while(found != std::string::npos){

      // Save the string between delimiters
      newline.push_back(line.substr(0, found));
      line.erase(0, found+1);

      // Check for the next delimiter
      found = line.find(",");
    }

    // Save the last string
    newline.push_back(line);
    line.clear();

    // Save the data found
    data.push_back(newline);
  }


  // Ommit the first line (header)
  size_t i=1;

  // Check the line is not void
  while(data[i][0].length() != 0){

    std::string aux_color = "NOCOLOR", aux_time = "1";

    // Check the color is given
    if(data[i][1].length() != 0){
      aux_color = data[i][1];
    }

    // Save the time name and color
    if(data[i][2].length() != 0){
      aux_time = data[i][2];
    }


    std::vector<std::string> aux_vec;
    aux_vec.push_back(data[i][0]);
    aux_vec.push_back(aux_color);
    aux_vec.push_back(aux_time);
    nodes.push_back(aux_vec);

    // Continue to next line
    i++;
  }

  // Ommit the void line and header
  i+=2;

  // Go 'till the end of the file
  while(i<data.size()){

    // Save the edge
    links.push_back(std::make_pair(data[i][0], data[i][1]));
    i++;
  }
}


int* create_networks(std::vector<std::vector<std::string> > &nodes, std::vector<std::pair<std::string, std::string> > &links){
  // Create the adjacency matrix
  int* matrix = new int[nodes.size()*nodes.size()]();

  // Locate the edges in the respective index
  for(size_t i=0; i<links.size(); i++){
    size_t j,k;

    // Get the index of first node
    for(j=0; j<nodes.size(); j++){
      if(links[i].first == nodes[j][0])
	break;
    }

    // Get the index of second node
    for(k=0; k<nodes.size(); k++){
      if(links[i].second == nodes[k][0])
	break;
    }

    // Create the edge in both directions
    matrix[j*nodes.size() + k] = 1;
    matrix[k*nodes.size() + j] = 1;
  }

  return matrix;
}


void DepthFirstSearch_DFS(int* matrix, int init, int end, size_t num_nodes, bool* visited, std::vector<int> &current_path, std::vector<std::vector<int> > &paths){
  // Check we won't start a cycle.
  // If the init node was visited, then is a cycle
  if(visited[init])
    return;

  // Check the init node as visited and save it in the current path
  visited[init] = true;
  current_path.push_back(init);

  // If the path has ended, then save it and exit
  // If the init and end nodes are same, then we have finished the path.
  if(init == end){

    // Save the current path
    paths.push_back(current_path);

    // Set the current (init) node as not visited and remove it from back
    visited[init] = false;
    current_path.pop_back();

    // Exit
    return;
  }

  // Check for the next possible node in the path
  for(size_t i=0; i<num_nodes; i++){

    // If the edge exit, then go to the node
    if(matrix[init*num_nodes + i] == 1)
      DepthFirstSearch_DFS(matrix, i, end, num_nodes, visited, current_path, paths);
  }

  // If the path ended, but it wasn't in the 'end' node, then turn back
  visited[init] = false;
  current_path.pop_back();
}


void modify_paths(std::vector<std::vector<int> > &paths, std::vector<std::vector<std::string> > &nodes){

  // Container of nodes 'color' ommit
  std::vector<int> no_nodes;

  // If 'color' is different to nocolor, collect prohibited nodes
  if(MyPar.color != "NOCOLOR"){
    for(size_t i=0; i<nodes.size(); i++){
      if(nodes[i][1] != "NOCOLOR" && nodes[i][1] != MyPar.color)
	no_nodes.push_back(i);
    }
  }

  // Loop over the paths to delete nodes in 'no_nodes'
  for(size_t i=0; i<paths.size(); i++){

    // Loop over each item in the path to check if there is a prohibited node
    for(size_t j=0; j<paths[i].size(); j++){

      // If a node without the appropiate color is in the path, erase it
      if(std::find(no_nodes.begin(), no_nodes.end(), paths[i][j]) != no_nodes.end()){
	paths[i].erase(paths[i].begin() + j);
	j--;
      }
    }
  }
}
