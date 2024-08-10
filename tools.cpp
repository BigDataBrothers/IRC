/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 16:26:53 by myassine          #+#    #+#             */
/*   Updated: 2024/08/10 21:15:26 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"


void error(std::string msg)
{
    std::cerr << msg << std::endl;
    exit(1);
}

bool is_numeric(const std::string& str) {
    return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}