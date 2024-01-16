/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgroux <vgroux@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/24 15:26:00 by vgroux            #+#    #+#             */
/*   Updated: 2023/12/06 16:44:45 by vgroux           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ShrubberyCreationForm.class.hpp"
#include "PresidentialPardonForm.class.hpp"
#include "RobotomyRequestForm.class.hpp"
#include "Bureaucrat.class.hpp"

int	main(void)
{
	ShrubberyCreationForm	shrub("Maokai");	// sign145 | exec137
	RobotomyRequestForm		robo("Lolo");		// sign072 | exec045
	PresidentialPardonForm	pres("Bober");		// sign025 | exec005

	Bureaucrat	thombre("Thombre", 1);
	Bureaucrat	saucisse("Saucisse", 50);
	Bureaucrat	beobim("Beobim", 150);

	std::cout << "-----------------" << std::endl;
	thombre.signForm(shrub);
	thombre.signForm(pres);
	beobim.signForm(robo); // Ne peut pas signer le formulaire

	beobim.executeForm(shrub); // GradeTooLow
	saucisse.executeForm(robo);// Unsigned

	thombre.signForm(robo);

	std::cout << "-----------------" << std::endl;
	saucisse.executeForm(shrub);
	std::cout << std::endl;
	thombre.executeForm(robo);
	std::cout << std::endl;
	thombre.executeForm(pres);

	std::cout << "-----------------" << std::endl;
	return 0;
}

-----------------------------31921719793027313661442879850--
