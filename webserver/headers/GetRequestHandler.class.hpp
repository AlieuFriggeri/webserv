/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetRequestHandler.class.hpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgroux <vgroux@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 16:58:38 by vgroux            #+#    #+#             */
/*   Updated: 2023/11/06 14:18:03 by vgroux           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_REQUEST_HANDLER_CLASS_HPP
# define GET_REQUEST_HANDLER_CLASS_HPP

# include "RequestHandler.class.hpp"

class GetRequestHandler: public RequestHandler
{
	private:
		
	public:
		GetRequestHandler(void);
		~GetRequestHandler(void);

		HttpRespond	handleRequest(HttpRequest req);
};

#endif
