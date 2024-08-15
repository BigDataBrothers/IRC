/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 16:26:53 by myassine          #+#    #+#             */
/*   Updated: 2024/08/15 19:02:39 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void error(std::string msg)
{
    std::cerr << "ERREUR:" << std::endl << msg << std::endl;
    exit(EXIT_FAILURE);
}

bool is_numeric(const std::string& str) {
    if (str.empty()) return false;
    for (std::string::const_iterator it = str.begin(); it != str.end(); ++it)
        if (!std::isdigit(*it))
            return false;
    return true;
}