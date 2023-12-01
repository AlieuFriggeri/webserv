/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgroux <vgroux@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:44:59 by vgroux            #+#    #+#             */
/*   Updated: 2023/11/30 17:47:57 by vgroux           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# include <string>
# include <sstream>
# include <iostream>
# include <iomanip>
# include <fstream>
# include <dirent.h>
# include <vector>

# define CRLN "\r\n"

std::string	getStatusStr(int statusCode);
std::string	toString(int i);
std::vector<char>	openReadFile(std::string relative_path);
std::vector<char>	openReadCloseDir(std::string path, std::string uri);

#endif
