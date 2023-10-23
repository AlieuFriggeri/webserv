/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.class.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgroux <vgroux@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 16:52:34 by vgroux            #+#    #+#             */
/*   Updated: 2023/10/23 17:00:54 by vgroux           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HANDLER_CLASS_HPP
# define REQUEST_HANDLER_CLASS_HPP

# include "HttpRequest.class.hpp"
# include "HttpRespond.class.hpp"

class RequestHandler
{
	private:
		
	public:
		RequestHandler(void);
		~RequestHandler(void);

		virtual HttpRespond	handleRequest(HttpRequest req) = 0;
		
};

#endif
