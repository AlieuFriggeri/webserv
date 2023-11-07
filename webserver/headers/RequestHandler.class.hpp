/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.class.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgroux <vgroux@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 16:52:34 by vgroux            #+#    #+#             */
/*   Updated: 2023/11/07 17:41:39 by vgroux           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HANDLER_CLASS_HPP
# define REQUEST_HANDLER_CLASS_HPP

// # include "ServerConfig.hpp"
# include "HttpRequest.class.hpp"
# include "HttpRespond.class.hpp"

class RequestHandler
{
	private:
		// CONFIG_SERVER	conf;

	public:
		RequestHandler(void);
		// RequestHandler( CONFIG DU SERVER );
		RequestHandler(const RequestHandler& src);
		RequestHandler& operator=(const RequestHandler& src);
		~RequestHandler(void);

		virtual HttpRespond	handleRequest(HttpRequest req) = 0;
};

#endif
