/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetRequestHandler.class.hpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afrigger <afrigger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 16:58:38 by vgroux            #+#    #+#             */
/*   Updated: 2023/11/07 15:06:45 by afrigger         ###   ########.fr       */
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

		static HttpRespond	handleRequest(HttpRequest req);
};

#endif
