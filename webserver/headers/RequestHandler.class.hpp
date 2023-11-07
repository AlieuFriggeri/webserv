/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.class.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afrigger <afrigger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 16:52:34 by vgroux            #+#    #+#             */
/*   Updated: 2023/11/07 15:12:13 by afrigger         ###   ########.fr       */
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

		//virtual static HttpRespond	handleRequest(HttpRequest req) = 0;
};

#endif
