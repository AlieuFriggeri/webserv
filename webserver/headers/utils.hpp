/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgroux <vgroux@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:44:59 by vgroux            #+#    #+#             */
/*   Updated: 2023/12/01 14:42:45 by vgroux           ###   ########.fr       */
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
# include <sys/stat.h>

# define CRLN "\r\n"

std::string	getStatusStr(int statusCode);
std::string	toString(int i);
std::string	openReadFile(std::string relative_path);
std::string	openReadCloseDir(std::string path, std::string uri);

#endif
